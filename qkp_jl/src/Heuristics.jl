module Heuristics

using JuMP
using Gurobi
using CPLEX
using Data
using Parameters
using LinearAlgebra

mutable struct stdFormVars
  x
  y  
end

export stdFormVars, greedy, localsearch, callbackHeuristic

function localsearch(inst::InstanceData, params::ParameterData, xprime, lbmax)

  N = inst.N 

  q = zeros(N)
  res = inst.C

  for i=1:N
    if (xprime[i]==1) 
      res -= inst.W[i]
    end
  end
  
  while (res >= 0)
    for i=1:N
      tot = inst.P[i,i]
      # println("tot = ", tot)
      for j=1:inst.N
	      if ((j != i) && (xprime[j] != 0)) 
	        tot += inst.P[i,j] + inst.P[j,i]
        end
      end
      q[i] = tot;
    end
    
    bgain = gaini = gainj = 0;
    for i=1:N
      if (xprime[i] == 0)
        if (inst.W[i] <= res)
	        gain = q[i]
          if (gain > bgain)
	          bgain = gain
            gaini = i
            gainj = -1
          end
	      else
	        for j=1:N
	          if (j == i) 
              continue
            end
	          if (xprime[j] == 0) 
              continue
            end
	          if (inst.W[i] - inst.W[j] <= res)
	            gain = q[i] - q[j] - (inst.P[i,j] + inst.P[j,i])
	            if (gain > bgain)
	              bgain = gain; gaini = i; gainj = j;
              end
            end
          end
        end
      end
    end
  
    if (bgain == 0) 
      break
    end

    xprime[gaini] = 1

    if (gainj != -1) 
      xprime[gainj] = 0
    end

    if (gainj != -1) 
      res += inst.W[gainj] - inst.W[gaini]
    else 
      res -= w[gaini]
    end

    if (res < 0) 
      printf("error\n")
      exit(-1)
    end
  
  end # end of while loop
  
  gain = 0
  res = inst.C
  for i=1:N
    if (xprime[i] == 0)
      continue
    end
    res -= inst.W[i];
    for j=1:inst.N
      if (xprime[j]==1)
        gain += inst.P[i,j]
      end
    end
  end
  
  if (res < 0)
    printf("error \n")
    exit(-1)
  end
  
  # println("possible solution", gain)
  
  if (gain > lbmax)
    lbmax = gain
    #for i=1:inst.N xstar[i] = xprime[i];
  end
  
  # println("lbmax = ",lbmax)

end

function greedy(inst::InstanceData, params::ParameterData)

    psum = 0;
    wsum = 0;
   
    x = zeros(inst.N)
    for i = 1:inst.N
        wsum += inst.W[i];
        for j = 1:inst.N
            psum += inst.P[i,j];
        end
    end
  
    ptot = psum;
    wtot = wsum;
  
    for i = 1:inst.N
        x[i] = 1
    end
  
    psum = ptot
    wsum = wtot
  
    mini = 0
    minp = 0

    while (mini != -1 && wsum > inst.C)
        mineff = ptot
        mini = -1
        for i = 1:inst.N
            if (x[i]==0)
                continue
            end
            pii = -inst.P[i,i]
            for j = 1:inst.N 
	        if (x[j]==1) 
                    pii += inst.P[j,i] + inst.P[i,j]
                end
            end
            eff = pii / inst.W[i]
            if (eff < mineff) 
	        mineff = eff
	        mini = i
	        minp = pii
            end
        end
        if (mini == -1)
            println("error")
            exit(-1)
        end
        i = mini
        x[i] = 0
        psum -= minp
        wsum -= inst.W[i]
        if (wsum <= inst.C) 
           break
        end
    end

    lb = psum;

    # println("lb = ",lb)
  
    lbmax = localsearch(inst, params, x, lb)

end

function callbackHeuristic(inst::InstanceData, params::ParameterData)

  if params.solver == "Gurobi"
    model = Model(Gurobi.Optimizer)
    #set_optimizer_attribute(model, "NonConvex", 2)
    set_optimizer_attribute(model, "TimeLimit", params.maxtime) # Time limit
    set_optimizer_attribute(model, "MIPGap", params.tolgap) # Relative MIP optimality gap
    set_optimizer_attribute(model, "NodeLimit", params.maxnodes) # MIP node limit
    set_optimizer_attribute(model, "Cuts", 0) # Global cut aggressiveness setting. 
    set_optimizer_attribute(model, "PreCrush", 1) # Controls presolve reductions that affect user cuts
    set_optimizer_attribute(model, "VarBranch", -1) # Controls the branch variable selection strategy. 
    set_optimizer_attribute(model, "NodeMethod", -1) # Method used to solve MIP node relaxations
    set_optimizer_attribute(model, "BranchDir", -1) # Preferred branch direction
    set_optimizer_attribute(model, "Presolve", -1) # Controls the presolve level
    set_optimizer_attribute(model, "Method", -1) # Algorithm used to solve continuous models or the root node of a MIP model. 
    set_optimizer_attribute(model, "Threads", 1) # Controls the number of threads.
  elseif params.solver == "Cplex"
    model = Model(Cplex.Optimizer)
  else
    println("No solver selected")
    return 0
  end

  N = inst.N

  ### Defining variables ###
  @variable(model, x[i=1:N], Bin)

  ### Objective function ###
  @objective(model, Max, 
  sum(inst.P[i,i]*x[i] for i=1:N) + 
  sum(inst.P[i,j]*x[i]*x[j] for i=1:N, j=(i+1):N)
  )

  ### knapsack constraints ###
  @constraint(model, knap, sum(inst.W[i]*x[i] for i=1:N) <= inst.C)

  #writeLP(model,"modelo.lp",genericnames=false)

  callback_called = false
  function heur_callback_function(cb_data)
      callback_called = true
      greedy(inst, params)
      x_vals = callback_value.(Ref(cb_data), x)
      ret = MOI.submit(model, MOI.HeuristicSolution(cb_data), x, x_vals)
      #println("Heuristic solution status = $(ret)")
  end

  MOI.set(model, MOI.HeuristicCallback(), heur_callback_function)

  optimize!(model)

  bestsol = objective_value(model)
  bestbound = objective_bound(model)
  numnodes = node_count(model)
  time = solve_time(model)
  gap = MOI.get(model, MOI.RelativeGap())

  open("saida.txt","a") do f
    write(f,"$(params.instName);$(params.form);$bestbound;$bestsol;$gap;$time;$numnodes;$(params.disablesolver) \n")
  end

  if params.printsol == 1
    printStandardFormulationSolution(inst,x)
  end

end 
 

end
