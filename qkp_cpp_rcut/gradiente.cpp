void gradiente_mq (const int n, const int *w, const int c, const double *x, double **y, double *grad, const int fgrad_mq) {
  register int i, j;
  
  for (j = 0; j < n; j++) {
    grad[j] = x[j] * c;
    grad[j] -= x[j] * w[j];
    
    for (i = 0; i < n; i++) {
      if (i < j) grad[j] -= w[i] * y[i][j];
      if (i > j) grad[j] -= w[i] * y[j][i];
    }
    /*grad[j] = grad[j] * 0.0002;*/
    grad[j] = grad[j]/(double)fgrad_mq;
  }
  
}

void gradiente_lini (const int n, const int nrl, const double *x, double **y, double *gradl, const bool *sitl) {
  register int i, j, t;
  
  t = 0;
  for (i = 0; i < n-1; i++) {
    for (j = i+1; j < n; j++) {
      if (sitl[t] == true) gradl[t] = x[i] - y[i][j];
      else gradl[t] = 0.0;
      t++;
    }
  }
  
  if (t != nrl) {
    printf ("erro : fora do intervalo \n");
    exit (-1);
  }
  
}

void gradiente_linj (const int n, const int nrl, const double *x, double **y, double *gradl, const bool *sitl) {
  register int i, j, t;
  
  t = 0;
  for (i = 0; i < n-1; i++) {
    for (j = i+1; j < n; j++) {
      if (sitl[t] == true) gradl[t] = x[j] - y[i][j];
      else gradl[t] = 0.0;
      t++;
    }
  }
  
  if (t != nrl) {
    printf("erro : fora do intervalo \n");
    exit (-1);
  }
  
}

void gradiente_linij (const int n, const int nrl, const double *x, double **y, double *gradl, const bool *sitl) {
  register int i, j, t;
  
  t = 0;
  for (i = 0; i < n-1; i++) {
    for (j = i+1; j < n; j++) {
      if (sitl[t] == true) gradl[t] = 1.0 + y[i][j] - x[i] - x[j];
      else gradl[t] = 0.0;
      t++;
    }
    }
  
  if (t != nrl) {
    printf("erro : fora do intervalo \n");
    exit (-1);
  }
  
}
