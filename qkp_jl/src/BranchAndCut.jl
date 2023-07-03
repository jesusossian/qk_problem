module BranchAndCut

#import MathOptInterface as MOI

using JuMP
using Gurobi
using CPLEX
using Data
using Parameters
using LinearAlgebra

import Heuristics

mutable struct stdFormVars
  x
  y  
end

export stdFormVars, callbackCutsQkp

function callbackCutsQkp(inst::InstanceData, params::ParameterData)

  if params.solver == "gurobi"
    model = Model(Gurobi.Optimizer)
    #set_optimizer_attribute(model, "NonConvex", 2)
    
    set_optimizer_attribute(model, "TimeLimit", params.maxtime) # Time limit
    set_optimizer_attribute(model, "MIPGap", params.tolgap) 
    # Relative MIP optimality gap
    
    #set_optimizer_attribute(model, "NodeLimit", params.maxnodes) # MIP node limit
    
    #set_optimizer_attribute(model, "Cuts", 0) 
    # Global cut aggressiveness setting. 
    # Use value 0 to shut off cuts, 
    # 1 for moderate cut generation, 
    # 2 for aggressive cut generation, and 
    # 3 for very aggressive cut generation. 
    # This parameter is overridden by the parameters that control individual cut types (e.g., CliqueCuts).
    # Only affects mixed integer programming (MIP) models
    
    #set_optimizer_attribute(model, "PreCrush", 1) 
    # Controls presolve reductions that affect user cuts
    
    #set_optimizer_attribute(model, "VarBranch", -1) 
    # Controls the branch variable selection strategy. 
    # The default -1 setting makes an automatic choice, depending on problem characteristics. 
    # Available alternatives are:
    # Pseudo Reduced Cost Branching (0), 
    # Pseudo Shadow Price Branching (1), 
    # Maximum Infeasibility Branching (2), and 
    # Strong Branching (3).
    
    #set_optimizer_attribute(model, "NodeMethod", -1) 
    # Method used to solve MIP node relaxations
    # Algorithm used for MIP node relaxations (except for the root node, see Method). 
    # Options are: 
    # -1=automatic, 
    # 0=primal simplex, 
    # 1=dual simplex, and 
    # 2=barrier. 
    # Note that barrier is not an option for MIQP node relaxations.
    
    #set_optimizer_attribute(model, "BranchDir", -1) # Preferred branch direction
    # Determines which child node is explored first in the branch-and-cut search. 
    # The default value chooses automatically. 
    # A value of -1 will always explore the down branch first, 
    # while a value of 1 will always explore the up branch first.
    
    #set_optimizer_attribute(model, "Presolve", -1) # Controls the presolve level
    # A value of -1 corresponds to an automatic setting. 
    # Other options are off (0), conservative (1), or aggressive (2). 
    # More aggressive application of presolve takes more time, 
    # but can sometimes lead to a significantly tighter model.
    
    #set_optimizer_attribute(model, "Method", -1) 
    # Algorithm used to solve continuous models or the root node of a MIP model. 
    # Options are: 
    # -1=automatic, 
    # 0=primal simplex, 
    # 1=dual simplex, 
    # 2=barrier, 
    # 3=concurrent, 
    # 4=deterministic concurrent, 
    # 5=deterministic concurrent simplex.
    
    set_optimizer_attribute(model, "Threads", 1)
    # Controls the number of threads to apply to parallel algorithms.
    # The default value of 0 is an automatic setting. 
    # It will generally use all of the cores in the machine, but it may choose to use fewer.
    
  elseif params.solver == "cplex"
    model = Model(Cplex.Optimizer)
  else
    println("No solver selected")
    return 0
  end

  N = inst.N

  ### Defining variables ###
  @variable(model, x[i=1:N], Bin)
  @variable(model, y[i=1:N, j=i+1:N], Bin)

  ### Objective function ###
  @objective(model, Max, 
  sum(inst.P[i,i]*x[i] for i=1:N) + 
  sum(inst.P[i,j]*y[i,j] for i=1:N, j=(i+1):N)
  )

  ### knapsack constraints ###
  @constraint(model, knap, sum(inst.W[i]*x[i] for i=1:N) <= inst.C)
  @constraint(model, lini[i=1:inst.N, j=(i+1):N], y[i,j] <= x[i])
  @constraint(model, linj[i=1:inst.N, j=(i+1):N], y[i,j] <= x[j])
  #@constraint(model, linij[i=1:inst.N, j=(i+1):N], x[i] + x[j] <= 1 + y[i,j])
	
  #writeLP(model,"modelo.lp",genericnames=false)

  cut_called = false
  function cut_callback_function(cb_data)
    cut_called = true
    x_vals = callback_value.(Ref(cb_data), x)
    y_vals = callback_value.(Ref(cb_data), y)
    for i=1:N
      for j=(i+1):N
        if (x_vals[i] + x_vals[j] > 1 + y_vals[i,j] + 1e-6) 
          con = @build_constraint(
            x[i] + x[j] <= 1 + y[i,j]
          )
          println("Adding $(con)")
          MOI.submit(model, MOI.UserCut(cb_data), con)
        end
      end
    end
  end

  lazy_called = false
  function lazy_callback_function(cb_data)
    lazy_called = true
    x_vals = callback_value.(Ref(cb_data), x)
    y_vals = callback_value.(Ref(cb_data), y)
    status_l = callback_node_status(cb_data, model)
    if status_l == MOI.CALLBACK_NODE_STATUS_FRACTIONAL
      println(" - Solution is integer infeasible!")
    elseif status_l == MOI.CALLBACK_NODE_STATUS_INTEGER
      println(" - Solution is integer feasible!")
    else
      @assert status_l == MOI.CALLBACK_NODE_STATUS_UNKNOWN
      println(" - I don't know if the solution is integer feasible :(")
    end
    for i=1:N
      for j=(i+1):N
        if (x_vals[i] + x_vals[j] > 1 + y_vals[i,j]) 
          con = @build_constraint(x[i] + x[j] <= 1 + y[i,j] + 1e-6)
          println("Adding $(con)")
          MOI.submit(model, MOI.LazyConstraint(cb_data), con)
        end
      end
    end
  end

  
  callback_called = false
  function heur_callback_function(cb_data)
    callback_called = true
    x_vals = Heuristics.greedy(inst, params)
    #x_vals = callback_value.(Ref(cb_data), x)
    status_h = MOI.submit(model, MOI.HeuristicSolution(cb_data), x, x_vals)
    println("Heuristic solution status = $(status_h)")
  end
  
  #MOI.set(model, MOI.UserCutCallback(), cut_callback_function)
  #MOI.set(model, MOI.LazyConstraintCallback(), lazy_callback_function)
  MOI.set(model, MOI.HeuristicCallback(), heur_callback_function)

  status = optimize!(model)

  opt = 0
  if termination_status(model) == MOI.OPTIMAL    
    println("status = ", termination_status(model))
    opt = 1
  else
    println("status = ", termination_status(model))
  end

  bestsol = objective_value(model)
  bestbound = objective_bound(model)
  numnodes = node_count(model)
  time = solve_time(model)
  gap = MOI.get(model, MOI.RelativeGap())

  open("saida.txt","a") do f
    write(f,"$(params.instName);$(params.form);$bestbound;$bestsol;$gap;$time;$numnodes;$opt \n")
  end

  #if params.printsol == 1
  #  printStandardFormulationSolution(inst,x)
  #end

end

end
