# qkp_codes

Quadratic Knapsack Problem

Consider

N =  \{1, 2, \ldots, n \} numbers of items

p_{i,j}, profit for i=1,2,\ldots,n and j=i+1,\ldots,n 

w_i, weigt for each i \in N

c, capacit 

Problem
\begin{align}
\max \ & \sum_{i=1}^{n} \sum_{j=i+1}^{n} p_{i,j} x_i x_j \\
& \sum_{i=1}^{n} w_i x_i \leq c \\
& x_i \in \{0, 1\} for each i \in N 
\end{align}

