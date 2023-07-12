push!(LOAD_PATH, "src/")
#push!(DEPOT_PATH, JULIA_DEPOT_PATH)
using Pkg
#Pkg.activate(".")
#Pkg.instantiate()
#Pkg.build()

using JuMP
using Gurobi
using CPLEX

import Data
import Parameters
import Formulations
import Heuristics
import cbBranchAndCut
import cbHeuristic
import cbLazyConstraint

# julia qknapsack.jl --inst AAAA --form BBBB 
 
# Read the parameters from command line
params = Parameters.readInputParameters(ARGS)

# Read instance data
inst = Data.readData(params.instName,params)

if params.form == "std"
    Formulations.stdFormulation(inst, params)
elseif params.form == "linear"
   Formulations.linearFormulation(inst, params)
elseif params.form == "greedy"
   xp = Heuristics.greedy(inst, params)
   #println(xp)
elseif params.form == "cbBCut"
   cbBranchAndCut.cbCuts(inst, params)
elseif params.form == "cbLazy"
   cbLazyConstraint.cbLazy(inst, params)
elseif params.form == "cbHeur"
   cbHeuristic.cbHeur(inst, params)
end
