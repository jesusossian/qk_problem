void guloso(const int n, int **p, const int *w, const int c, int *lb, int *x) {
  register int i, j;
  int mini;
  int ptot, wtot;
  int psum, wsum, pi, minp;
  double eff, mineff;

  psum = 0;
  wsum = 0;

  for (i = 0; i < n; i++) {
    x[i] = 0;
    wsum += w[i];
    for (j = 0; j < n; j++)
      psum += p[i][j];
  }
  
  ptot = psum;
  wtot = wsum;
  
  for (i = 0; i < n; i++) x[i] = 1;
  
  psum = ptot;
  wsum = wtot;
  
  for (;;) {
    mineff = ptot;
    mini = -1;
    for (i = 0; i < n; i++) {
      if (!x[i]) continue;
      pi = -p[i][i];
      for (j = 0; j < n; j++) 
	if (x[j]) pi += p[j][i] + p[i][j];
      eff = pi / (double) w[i];
      if (eff < mineff) {
	mineff = eff;
	mini = i;
	minp = pi;
      }
    }
    if (mini == -1) {
      printf("error \n");
      exit(-1);
    }
    i = mini;
    x[i] = 0;
    psum -= minp;
    wsum -= w[i];
    if (wsum <= c) break;
  }
  *lb = psum;
  
  melhora(n, p, w, c, x, lb);
  
}
