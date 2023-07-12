module cbBranchAndCut

using JuMP
using Gurobi
using CPLEX
using Data
using Parameters
using LinearAlgebra

#import MathOptInterface as MOI
#import Heuristics

mutable struct stdFormVars
    x
    y  
end

export stdFormVars, cbCuts

function cbCuts(inst::InstanceData, params::ParameterData)

    if params.solver == "gurobi"
        model = Model(Gurobi.Optimizer)
        #set_optimizer_attribute(model, "NonConvex", 2)
        set_optimizer_attribute(model, "TimeLimit", params.maxtime)
        set_optimizer_attribute(model, "MIPGap", params.tolgap) 
        #set_optimizer_attribute(model, "NodeLimit", params.maxnodes)
        #set_optimizer_attribute(model, "Cuts", 0)     
        #set_optimizer_attribute(model, "PreCrush", 1) 
        #set_optimizer_attribute(model, "VarBranch", -1)     
        #set_optimizer_attribute(model, "NodeMethod", -1)     
        #set_optimizer_attribute(model, "BranchDir", -1)
        #set_optimizer_attribute(model, "Presolve", -1)
        #set_optimizer_attribute(model, "Method", -1) 
        set_optimizer_attribute(model, "Threads", 1)
    
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
                    con = @build_constraint(x[i] + x[j] <= 1 + y[i,j])
                    #println("Adding $(con)")
                    MOI.submit(model, MOI.UserCut(cb_data), con)
                end
            end
        end
    end
    
    #MOI.set(model, MOI.UserCutCallback(), cut_callback_function)

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
    #    printStandardFormulationSolution(inst,x)
    #end

end

end
