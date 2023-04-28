#ifndef CABECALHO_H_INCLUDED
#define CABECALHO_H_INCLUDED

#include <ilcplex/ilocplex.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "aloca.cpp"

#define MAX_CPU_TIME 3600.0
#define EPSILON 0.000001
#define INFTY 1000000000.0

using namespace std;

typedef struct opcoes {
  int solve_lp, solve_mip;
} str_opc;

str_opc opc;

double *sx, **sy;

ILOSTLBEGIN
typedef IloArray<IloIntVarArray> IntMatrix;
typedef IloArray<IloNumVarArray> FloatMatrix;

double gap;
double best_bound;

string IntToString (int a);
double optimization (int N, int **P, int *W, int C, int *elapsed);
void print_instances (int N, int **P, int *W, int C);

#include "optimization.cpp"
#include "print_instances.cpp"

#endif
CABECALHO_H_INCLUDED
