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

export stdFormVars, greedy, localSearch

function localSearch(inst::InstanceData, params::ParameterData, xprime, lbmax)

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
            for j=1:N
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
            res -= inst.W[gaini]
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
        for j=1:N
            if (xprime[j]==1)
                gain += inst.P[i,j]
            end
        end
    end
  
    if (res < 0)
        printf("error \n")
        exit(-1)
    end
  
    #println("possible solution", gain)
  
    if (gain > lbmax)
        lbmax = gain
        #for i=1:N xstar[i] = xprime[i]
    end
  
    #println("lbmax = ", lbmax)
    return lbmax

end

function greedy(inst::InstanceData, params::ParameterData)

    psum = 0;
    wsum = 0;
    
    time = 0
   
    t1 = time_ns()

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
  
    lbmax = localSearch(inst, params, x, lb)
    
    t2 = time_ns()
    time = (t1-t2)/1.0e9
    
    open("saida.txt","a") do f
        write(f,"$(params.instName);$(lb);$(lbmax);$(time) \n")
    end

    return x

end

end
