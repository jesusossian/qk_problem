void print_instances (int N, int **P, int *W, int C) {

  printf("N = %d \n", N);
  
  for (int i=0; i<N; i++) printf("%d ", P[i][i]);
  printf("\n");
  
  for (int i=0; i<N; i++) {
    for (int j=i+1; j<N; j++) {
      printf("%d ", P[i][j]);
    }
    printf("\n");
  }
  
  printf("C = %d \n", C);
  
  for (int i=0; i<N; i++) printf("%d ", W[i]);
  printf("\n");

}
