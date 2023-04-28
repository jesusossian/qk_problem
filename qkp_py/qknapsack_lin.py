import sys
import numpy as np
import gurobipy as gp
from gurobipy import GRB

def qknapsack(datafile):
  with open(datafile, 'r') as file: linhas = file.readlines()

  linhas = [a.strip() for a in linhas] # remove linha vazia inicial e elimina os "\n" de cada linha

  n = int(linhas[0]) # ler o tamanho da instancia

  d = np.fromstring(linhas[1], dtype=int, sep = ' ') # ler a diagonal da matriz

  p = np.zeros((n,n), dtype=int) # define a matriz

  for i in range(n): # preenche a diagonal
    p[i][i] = d[i]

  for i in range(n-1): # preenche o resto da matriz
    linha = np.fromstring(linhas[i+2], dtype=int, sep = ' ')
    for j in range(n-(i+1)):
      p[i][j+i+1] = linha[j]
      p[j+i+1][i] = p[i][j+i+1]

  c = int(linhas[n+2]) # ler a capacidade

  w = np.fromstring(linhas[n+3], dtype=int, sep = ' ') # ler os pesos

  m = gp.Model("qknapsack") #cria o modelo

  x = m.addVars(n, vtype=GRB.BINARY, name='x') #Adicionando Variáveis 

  l = list(tuple())
  for i in range(0, n):
    for j in range(i+1, n):
      l.append((i,j))

  y = m.addVars(l, vtype=GRB.BINARY, name='y') #Adicionando Variáveis 

  obj = None
  for i in range(0, n):
    obj += p[i][i] * x[i]
    for j in range(i+1, n):
      obj += 2*p[i][j] * y[i,j]

  m.setObjective(obj, GRB.MAXIMIZE)

  constr0 = None
  for j in range(0, n):
    constr0 += (w[j] * x[j])
  m.addConstr(constr0 <= c)

  for i in range(0,n):
    for j in range(i+1, n):
      constr1 = y[i,j]
      m.addConstr(constr1 <= x[i])

  for i in range(0,n):
    for j in range(i+1, n):
      constr2 = y[i,j]
      m.addConstr(constr2 <= x[j])

  for i in range(0,n):
    for j in range(i+1, n):
      constr3 = x[i] + x[j]
      m.addConstr(constr3 <= 1 + y[i,j])

  m.write("qknapsack.lp")

  m.optimize()

  print('Obj: %g' % obj.getValue())

if __name__ == "__main__":
  datafile = "instances/10/10_25_1.txt"

  if len(sys.argv) < 2:
    print("Default data file : " + datafile)
  else:
    datafile = sys.argv[1]

  qknapsack(datafile)
