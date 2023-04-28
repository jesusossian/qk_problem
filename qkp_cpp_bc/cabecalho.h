#ifndef CABE
#define CABE

#include <ilcplex/ilocplex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "array.cpp"

#include <math.h>
#include <time.h>
#include <iostream>
#include <vector>

#define EPS    1e-6
#define INFTY 1000000000;

/* Constantes para definir estratégias de corte */
#define MAX_NUM_CORTES 2000   /* limite maximo de cortes */
#define MAX_ITER_SEP 5   /* numero maximo de iteracoes de corte por nodo */
#define EPSILON 0.000001 /* uma tolerância usada em alguns cálculos com doubles*/

/* limitando o tempo de CPU */
#define MAX_CPU_TIME -43200  /* colocar negativo ! (veja manual do XPRESS) */

using namespace std;

/****DATA****/
int N;
//Array<int> P;
//Array<Array<int> > PIJ;
//Array<int> W;
//int C;
//int ncutotal;
//int nit, qtdnodes;

//Array<double> cx;
//double best_sol;
//int nfixtotal, nfix0, nclique, nfixax, nfixay, qfx, qfy;

//Array< Array<int> > p;
static int ptot, wtot;
//Array<int> xprime;

int xpress_ret; /* valor de retorno das rotinas do XPRESS */
int xpress_status; /* status retornado por algumas rotinas do XPRESS */

/* OPCOES */
//int pr_lp;
//int solv_op;
//int eq_ln;
//int eq_trig0;
//int eq_trig1;

//int fixavx;
//int fixavy;
//int cliquecr;

//struct pxpress {
//  XPRSprob cover;
//  XPRSprob lift;
//};

/* OPCOES DO B&C */
//int eq_mochquad;
//int eq_tri;
//int eq_cover;
//int eq_extender;
//int eq_lift;
//int eq_cquad;
//int eq_equad;
//int eq_lquad;

//Array<double> x;
//Array<double> xz;
//Array<double> z;
//Array<double> xl;
//Array<int> xp;
//Array<int> stx;
//Array<Array <int> > sty;
//Array<Array <double> > cr;

//double opt, boundr, bound0, boundb;
//int cut0, qtdcut;
//int cutclique;
//double tempo;

//int nodeold = 0;
//int itnode = 0;

//int main(int argc, char **argv);
//void modelo(XPRSprob );
//void modelo_cover(pxpress *);
//void modelo_lift(pxpress *);
//double separacao_cover(pxpress *);
//int adicionar_bc_lin(XPRSprob , Array<int> &, Array<char> &, Array<double> &, Array <int> &, Array<int> &, Array<double> &);
//int adicionar_bc_tri(XPRSprob, Array<int> &, Array<char> &, Array<double> &, Array<int> &, Array<int> &, Array<double> &);
//int adicionar_bc_cover(XPRSprob , Array<int> &, Array<char> &, Array<double> &, Array<int> &, double , Array<int> &, Array<double> &);
//int adicionar_bc_cover_quad(XPRSprob, Array<int> &, Array<char> &, Array<double> &, Array<int> &, double , Array<int> &, Array<double> &);
//int adicionar_bc_extender(XPRSprob , Array<int> &, Array<char> &, Array<double> &, Array<int> &, double , Array<int> &, Array<double> &);
//int adicionar_bc_extender_quad(XPRSprob, Array<int> &, Array<char> &, Array<double> &, Array<int> &, double , Array<int> &, Array<double> &);
//int adicionar_bc_lift(XPRSprob , Array<int> &, Array<char> &, Array<double> &, Array<int> &, double , Array<int> &, Array<double> &, pxpress * );
//int adicionar_bc_lift_quad(XPRSprob , Array<int> &, Array<char> &, Array<double> &, Array<int> &, double , Array<int> &, Array<double> &, pxpress * );
//double resolvecovercall(XPRSprob );
//char * itoa(int ) ;

//void ordena_d(int *, int , int ,  int *);
//int particao(int *, int , int , int *);
//void troca(int *, int *);
//void fixaxy(XPRSprob, double , double, Array<int> &);
//void fixay(XPRSprob, double , double, Array<int> &);
//void fix_xpress(XPRSprob , const Array<int> &, int);
//void greedy();
//void improve(Array<int> &);
//void clique_cr(XPRSprob , Array<int> &, Array<char> &, Array<double> &, Array <int> &, Array<int> &, Array<double> &, double , double);

//#include "modelo.cpp"
//#include "modelo_cover.cpp"
//#include "modelo_lift.cpp"
//#include "resolve_cover_callback.cpp"
//#include "separacao_cover.cpp"
//#include "adicionar_bc.cpp"
//#include "ordena_d.cpp"
//#include "particao.cpp"
//#include "troca.cpp"
//#include "fixacao.cpp"
//#include "heuristica.cpp"

#include "aloca.cpp"

#endif
