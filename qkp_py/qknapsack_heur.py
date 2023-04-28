import sys
#import numpy as np
#import gurobipy as gp
#from gurobipy import GRB

from copy import deepcopy as dp
from glob import glob
#from tqdm import tqdm
import numpy as np
import re

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

  return n, p, w, c

def guloso(n, p, w, c):
  psum = wsum = 0
  
  x = [None for i in range(n)]
  
  for i in range(n):
    x[i] = 0
    wsum += w[i]
    for j in range(n):
      psum += p[i][j]
  
  ptot = psum
  wtot = wsum
  
  for i in range(n):
    x[i] = 1
    
  while True:
    mineff = ptot
    mini = -1
    for  i in range(n):
      if x[i] == 0:
        continue
      pi = -p[i][i]
      for j in range(n):
        if x[j] != 0:
          pi += p[j][i] + p[i][j]

      eff = pi / w[i]
      if eff < mineff:
        mineff = eff
        mini = i
        minp = pi
    if mini == -1:
      print("error\n")
      exit
    i = mini
    x[i] = 0
    psum -= minp
    wsum -= w[i]
    if wsum <= c:
      break
  lb = psum

  #print(lb, x)
  print('Resultado do guloso')
  print(lb, x, '\n')

  #melhora(n, p, w, c, x, lb)
  
  return lb, x

def melhora(n, p, w, c, xprime, lb):

  xstar = [None for i in range(n)]

  res = c
  
  q = [None for i in range(n)]
  
  for i in range(n):
    if xprime[i] != 0:
      res -= w[i]
  
  while True:
    for i in range(n):
      tot = p[i][i]
      for j in range(n):
        if (j != i) and (xprime[j] != 0):
          tot += p[i][j] + p[j][i]
      q[i] = tot
    
    bgain = gaini = gainj = 0
    
    for i in range(n):
      if xprime[i] == 0:
        if w[i] <= res:
          gain = q[i]
          if gain > bgain:
              bgain = gain
              gaini = i
              gainj = -1
        else:
          for j in range(n):
            if j == i:
              continue
            if xprime[j] == 0:
              continue
            if w[i] - w[j] <= res:
              gain = q[i] - q[j] - (p[i][j] + p[j][i])
              if gain > bgain:
                bgain = gain
                gaini = i
                gainj = j
    if bgain == 0:
      break
    
    xprime[gaini] = 1
    
    if gainj != -1:
      xprime[gainj] = 0
      res += w[gainj] - w[gaini]
    else:
      res -= w[gaini]
    
    if res < 0:
      print("error\n")
      exit
  gain = 0
  res = c
  for i in range(n):
    if xprime[i] == 0:
      continue
    res -= w[i]
    for j in range(n):
      if xprime[j] != 0:
        gain += p[i][j]

  if res < 0:
    print("error \n")
    exit 

  if gain > lb:
    lb = gain
    for i in range(n):
      xstar[i] = xprime[i]
  del(q)
  print('Resultado do melhora')
  print(lb, xstar)
  #return lb, xstar


if __name__ == "__main__":
  datafile = "instances/10/10_25_1.txt"

  if len(sys.argv) < 2:
    print("Default data file : " + datafile)
  else:
    datafile = sys.argv[1]

  n, p, w, c = qknapsack(datafile)

  lb, x = guloso(n, p, w, c)

  melhora(n, p, w, c, x, lb)
