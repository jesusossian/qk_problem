#!/bin/bash

mip_=mip
solver_=gurobi # cplex
form_=std

for tam in 10 #10 50 100 200 300 400
do
    for dens in 25 50 75 100
    do
        for id in {1..10} #$(seq 10)
        do
            julia qknapsack.jl --inst ../instances/${tam}/${tam}_${dens}_${id}.txt --form ${form_} --solver ${solver_} $ >> report/${form_}_${tam}_${dens}_${id}.txt
        done
    done
    mv saida.txt result/${form_}_${tam}.txt
done
