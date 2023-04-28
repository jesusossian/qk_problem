#include "cabecalho.h"

int main (int argc, char** argv) {
  int N;
  register int i, j;
  FILE* fp;
  FILE* fpout;
  time_t start, end;
  int elapsed;
  double ubmin;
  int **P;
  int *W;
  int C;
  int **p;
  int *w;
  int c, n;
   
  lagbest = 0.0;
  lbmax = 0;
  
  opc.hbillionnet = 0;
  opc.guloso_bl = 0;

  if (argc < 2 || argc > 20) {
    printf("numero de parametros de entrada errado \n");
    return 0;
  }
  
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "nao pode abrir %s \n", argv[1]);
    return 0;
  }
  
  fscanf(fp, "%d", &N);
  
  W = malloc_vetor_int(N);
  P = malloc_matriz_int(N,N);
  
  for (i = 0; i < N; i++) fscanf(fp, "%d", &(P[i][i]));
  
  for (i = 0; i < N-1; i++) {
    for (j = i+1; j < N; j++) {
      fscanf(fp, "%d", &(P[i][j]));
    }
  }
  
  fscanf(fp, "%d", &C);
  
  for (i = 0; i < N; i++) fscanf(fp, "%d", &(W[i]));
  
  fclose(fp);
  
  fp = NULL;
  
  for (i = 2; i < argc; i++) {
    if (strcmp(argv[i], "-hbillionnet") == 0) {
      opc.hbillionnet = 1;
    } else if (strcmp(argv[i], "-guloso_bl") == 0) {
      opc.guloso_bl = 1;
    } else {
      printf("parametros de entrada errado \n");
    }
  }
  
  n = N;
  c = C;
  
  for (i = 0; i < n-1; i++) {
    for (j = i+1; j < n; j++) {
      P[i][j] *= 2;
    }
  }
  
  p = calloc_matriz_int(n,n);
  
  for (i = 0; i < n; i++) {
    for (j = i; j < n; j++) {
      p[i][j] = P[i][j];
    }
  }
  
  w = calloc_vetor_int(n);
  for (i = 0; i < n; i++) {
    w[i] = W[i];
  }
  
  pbest = calloc_matriz_double(n,n);
  for (i = 0; i < n; i++) {
    for (j = i; j < n; j++)
      {
	pbest[i][j] = p[i][j];
      }
  }
  
  xstar = calloc_vetor_int(n);
  x = calloc_vetor_int(n);
 
  start = time(NULL);
   
  if (opc.hbillionnet == 1 || opc.guloso_bl == 1) {
    guloso(n, p, w, c, &lbmax, x);
    for (i = 0; i < n; i++) {
      xstar[i] = x[i];
    }  
  }
  
  printf("++++++++++ instancia %s ++++++++++++\n",  argv[1]);
  
  subgradiente (n, p, w, c, &ubmin, &lbmax);
    
  printf("lbmax %d, ubmin %.2f \n", lbmax, ubmin);
  
  end = time(NULL);
  elapsed = difftime(end, start);
  
  fpout = fopen ("saida.txt", "a");
  fprintf (fpout, "%s;%d;%.2f;%d \n", argv[1], lbmax, ubmin, elapsed);
  fclose (fpout);
  
  for (i = 0; i < N; i++) {
    free(P[i]);
    P[i] = NULL;
  }
  free(P); P = NULL;
  free(W); W = NULL;

  for (i = 0; i < N; i++) {
    free(p[i]);
    p[i] = NULL;
  }
  free(p); p = NULL;
  
  free(w); w = NULL;
  
  free(xstar); xstar = NULL;

  for (i = 0; i < N; i++) {
    free(pbest[i]);
    pbest[i] = NULL;
  }
  free(pbest); pbest = NULL;

  free(x); x = NULL;
  
  return 0;
  
}
