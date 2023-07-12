#!/bin/bash

mip_=mip
solver_=gurobi # cplex
form_=linear

for tam in 400 #10 50 100 200 300 400
do
    for dens in 100 # 25 50 75 100
    do
        for id in {1..10}
        do
            julia qknapsack.jl --inst ../instances/${tam}/${tam}_${dens}_${id}.txt --form ${form_} --solver ${solver_} $ >> report/${form_}_${tam}_${dens}_${id}.txt
        done
        mv saida.txt result/${form_}_${tam}_${dens}.txt
    done
done
