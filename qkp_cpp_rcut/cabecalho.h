#define EPSILON 10E-9
#define INFTY 100000000
#define MSIZE 1000
#define TMAX 43200
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <stack>
#include <iostream>
#include <vector>
#include <list>

#ifndef READDATE_H
#define	READDATE_H

#define SWAP(a, b) { lpitem t; t = *(a); *(a) = *(b); *(b) = t; }
#define DET(a1, a2) ((a1) - (a2))
#define DET1(a1, a2) ((a2) - (a1))

typedef struct _lpitem {
  double p;
  int w;
  double e;
  double *x;
} lpitem;

typedef struct _heuritem {
  int w;
  double e;
  int *x;
} heuritem;

typedef struct opcoes {
  int hbillionnet, guloso_bl;
} str_opc;

typedef int (*funcptr) (const void *, const void *);

str_opc opc;
int *xstar;
double **pbest;
double lagbest;
int z0, lbmax;
int nite;
int *x;

using namespace std;

void sort (lpitem *, lpitem *);
lpitem *partesort (lpitem *, lpitem *);

void custolag_mq (const int n, const int *w, const int c, double **p, const double *lag);
void custolag_lini (const int n, int const nrl, double **p, const double *lagl);
void custolag_linj (const int n, const int nrl, double **p, const double *lagl);
void custolag_linij (const int n, const int nrl, double **p, const double *lagl);

double proby (const int n, double **y, double **p);
void add_proby (lpitem *a, const int n, const int c, double **p);
void sol_y (const int n, double **y, double **p, const double *xd);

void gradiente_mq (const int n, const int *w, const int c, const double *x, double **y, double *grad, const int fgrad_mq);
void gradiente_lini (const int n, const int nrl, const double *x, double **y, double *gradl, const bool *sitl);
void gradiente_linj (const int n, const int nrl, const double *x, double **y, double *gradl, const bool *sitl);
void gradiente_linij (const int n, const int nrl, const double *x, double **y, double *gradl, const bool *sitl);

void dualiza_lini (const int n, const int nrl, const double *x, double **y, const double *lagl, bool *sitl, int *svidl, const int nvidl);
void dualiza_linj (const int n, const int nrl, const double *x, double **y, const double *lagl, bool *sitl, int *svidl, const int nvidl);
void dualiza_linij (const int n, const int nrl, const double *x, double **y, const double *lagl, bool *sitl, int *svidl, const int nvidl);

void lagrange (const int nr, const double *grad, double *lag, const double step);

double quadgrad_mq (const int n, const double *grad);
double quadgrad (const int nr, const double *grad, const bool *sit);

void guloso(const int n, int **p, const int *w, const int c, int *lb, int *x);
void melhora (const int n, int **p, const int *w, const int c, int *xprime, int *lb);

double lpsolve (lpitem *a, const int c, const int n);

void subgradiente (int loop, int n, int **p, int *w, int c, double *ubmin);

int icomp (int *a, int *b);

double** malloc_matriz_double (const int m, const int n);
int** malloc_matriz_int (const int m, const int n);
bool* malloc_vetor_bool (const int n);
double* malloc_vetor_double (const int n);
int* malloc_vetor_int (const int n);

double** calloc_matriz_double (const int m, const int n);
int** calloc_matriz_int (const int m, const int n);
bool* calloc_vetor_bool (const int n);
double* calloc_vetor_double (const int n);
int* calloc_vetor_int (const int n);

#include "aloca.cpp"
#include "ordenacao.cpp"
#include "subgradiente.cpp"
#include "custolag.cpp"
#include "gradiente.cpp"
#include "dualiza.cpp"
#include "lagrange.cpp"
#include "quadgrad.cpp"
#include "guloso.cpp"
#include "melhora.cpp"
#include "resolve_qkp.cpp"

#endif	/* READDATE_H */
