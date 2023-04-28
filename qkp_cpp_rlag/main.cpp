#include "cabecalho.h"

int main (int argc, char** argv) {
  int N;
  register int i, j, h, k;
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
  
  lbmax = 0;
  
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
    for (j = i; j < n; j++) {
      pbest[i][j] = p[i][j];
    }
  }
  
  xstar = calloc_vetor_int(n);
  x = calloc_vetor_int(n);
  
  start = time(NULL);
  
  
  guloso(n, p, w, c, &lbmax, x);
  for (i = 0; i < n; i++) {
    xstar[i] = x[i];
  }
  
  printf("++++++++++ instancia %s ++++++++++++\n",  argv[1]);
  
  subgradiente (h, n, p, w, c, &ubmin, &lbmax);
  
  end = time(NULL);
  elapsed = difftime(end, start);
  
  fpout = fopen ("saida.txt", "a");
  fprintf (fpout, "%s;%d;%d;%.2f;%d \n", argv[1], lbmax, z0, ubmin, elapsed);
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
  
  if (w) {
    free(w); w = NULL;
  }
  
  free(xstar);  xstar = NULL;
  free(x); x = NULL;
  
  return 0;
  
}
