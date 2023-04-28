void custolag_mq (const int n, const int *w, const int c, double **p, const double *lag) {
  register int i, j;
  
  for (i = 0; i < n; i++) {
    p[i][i] += lag[i]*(c - w[i]);
  }
  
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      if (i < j) p[i][j] -= lag[j]*w[i];
      if (i > j) p[j][i] -= lag[j]*w[i];
    }
  }
  
}

void custolag_lini (const int n, int const nrl, double **p, const double *lagl) {
  register int t, i, j;
  
  t = 0;
  for (i = 0; i < n-1; i++) {
    for (j = i+1; j < n; j++) {
      p[i][i] += lagl[t];
      p[i][j] -= lagl[t];
      t++;
    }
  }
  
  if (t != nrl) {
    printf ("erro : fora do intervalo \n");
    exit (-1);
  }
  
}

void custolag_linj (const int n, const int nrl, double **p, const double *lagl) {
  register int t, i, j;
  
  t = 0; 
  for (i = 0; i < n-1; i++) {
    for (j = i+1; j < n; j++) {
      p[j][j] += lagl[t];
      p[i][j] -= lagl[t];
      t++;
    }
  }
  
  if (t != nrl) {
    printf ("erro : fora do intervalo \n");
    exit (-1);
  }
  
}

void custolag_linij (const int n, const int nrl, double **p, const double *lagl) {
  register int t, i, j;
  
  t = 0; 
  for (i = 0; i < n-1; i++) {
    for (j = i+1; j < n; j++) {
      p[i][i] -= lagl[t];
      p[j][j] -= lagl[t];
      p[i][j] += lagl[t];
      t++;
    }
  }
  
  if (t != nrl) {
    printf("erro : fora do intervalo \n");
    exit(-1);
  }
  
}
