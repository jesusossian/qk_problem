double quadgrad_mq (const int n, const double *grad) {
  register int i;
  double d;  
  
  d = 0.0;
  for (i = 0; i < n; i++) {
    d += grad[i]*grad[i];
  }
  
  return d;
  
}

double quadgrad (const int nr, const double *grad, const bool *sit) {
  register int i;
  double d;
  
  d = 0.0;
  for (i = 0; i < nr; i++) {
    if (sit[i] == true) d += grad[i]*grad[i];
  }
  
  return d;
  
}
