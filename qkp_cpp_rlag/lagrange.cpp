void lagrange (const int nr, const double *grad, double *lag, const double step) {
  register int i;
  
  for (i = 0; i < nr; i++) {
    if ((lag[i] - step * grad[i]) > EPSILON) {
      lag[i] = lag[i] - step * grad[i];
    } else {
      lag[i] = 0.0;
    }
  }
  
}
