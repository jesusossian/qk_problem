push!(LOAD_PATH, "src/")
# push!(DEPOT_PATH, JULIA_DEPOT_PATH)
using Pkg
#Pkg.activate(".")
# Pkg.instantiate()
# Pkg.build()

using JuMP
using Gurobi
using CPLEX

import Data
import Parameters
import Formulations
import Heuristics
import BranchAndCut

# Read the parameters from command line
params = Parameters.readInputParameters(ARGS)

# Read instance data
inst = Data.readData(params.instName,params)

if params.form == "std"
    Formulations.stdFormulation(inst, params)
elseif params.form == "linear"
   Formulations.linearFormulation(inst, params)
elseif params.form == "greedy"
   Heuristics.greedy(inst, params)
elseif params.form == "bc"
   BranchAndCut.callbackCutsQkp(inst, params)
elseif params.form == "cbheur"
   Heuristics.callbackHeuristic(inst, params)
end
