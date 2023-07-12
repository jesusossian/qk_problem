module Parameters

struct ParameterData
    instName::String
    form::String
    solver::String
    maxtime::Int
    tolgap::Float64
    method::String
    #printsol::Int
    #disablesolver::Int
    #maxnodes::Int
end

export ParameterData, readInputParameters

function readInputParameters(ARGS)

    ### Set standard values for the parameters ###
    instName = "../instances/50/50_100_1.txt"
    form = "greedy"
    solver = "gurobi"
    maxtime = 3600
    tolgap = 1e-6
    method = "mip" # lp
    #printsol = 0
    #disablesolver = 0
    #maxnodes = 1000000000000

    ### Read the parameters and set correct values whenever provided ###
    for param in 1:length(ARGS)
        if ARGS[param] == "--inst"
            instName = ARGS[param+1]
            param += 1
        elseif ARGS[param] == "--form"
            form = ARGS[param+1]
            param += 1
        elseif ARGS[param] == "--solver"
            solver = ARGS[param+1]
            param += 1
        elseif ARGS[param] == "--maxtime"
            maxtime = parse(Int,ARGS[param+1])
            param += 1
        elseif ARGS[param] == "--tolgap"
            tolgap = parse(Float64,ARGS[param+1])
            param += 1
        elseif ARGS[param] == "--method"
            method = parse(ARGS[param+1])
            param += 1
        #elseif ARGS[param] == "--printsol"
        #    printsol = parse(Int,ARGS[param+1])
        #    param += 1
        #elseif ARGS[param] == "--disablesolver"
        #    disablesolver = parse(Int,ARGS[param+1])
        #    param += 1
        #elseif ARGS[param] == "--maxnodes"
        #    maxnodes = parse(Float64,ARGS[param+1])
        #    param += 1
        end
    end

    params = ParameterData(instName,form,solver,maxtime,tolgap,method)#,printsol,disablesolver,maxnodes)

    return params

end

end
