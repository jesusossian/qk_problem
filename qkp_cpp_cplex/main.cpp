#include "cabecalho.h"

int main (int argc, char** argv) {
  int N;
  FILE* fp;
  int **P;
  int *W;
  int C;
  double objval;
  int elapsed; 
  
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "can't open file %s \n", argv[1]);
    return 0;
  }
  
  if (fscanf(fp, "%d", &N) != 1);
  
  W = calloc_vetor_int(N);
  P = calloc_matriz_int(N,N);
  sx = calloc_vetor_double(N);
  sy = calloc_matriz_double(N,N);
  
  for (int i=0; i<N; i++) {
    if (fscanf(fp, "%d", &(P[i][i])) != 1);
  }
  
  for (int i=0; i<N-1; i++) {
    for (int j = i+1; j < N; j++) {
      if (fscanf(fp, "%d", &(P[i][j])) != 1);
    }
  }
  
  if (fscanf(fp, "%d", &C) != 1);
  
  for (int i=0; i<N; i++) {
    if (fscanf(fp, "%d", &(W[i])) != 1);
  }
  
  fclose(fp);
  fp = NULL;

  //print_instances(N,P,W,C);
  
  objval = 0.0;
  elapsed = 0;
  
  objval = optimization(N,P,W,C,&elapsed);
  
  printf("objval = %.1f \n", objval);

  free_matriz_int(N,P);
  free_vetor_int(W);
  free_vetor_double(sx);
  free_matriz_double(N,sy);
  
  return 0;
  
}
