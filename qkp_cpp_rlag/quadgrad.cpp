double quadgrad (const int nr, const double *grad) {
  register int i;
  double d;
  
  d = 0.0;
  for (i = 0; i < nr; i++) {
    d += grad[i]*grad[i];
  }
  
  return d;

}
