double lpsolve_fb (lpitem *a, const int c, const int n) {
  lpitem *k, *m;
  register int r;
  double ps;
  
  if (c <= 0) return 0;
  m = a + n;

  sort (a, m-1);

  for (k = a, ps = 0, r = c;; k++) {
    //if (k->p <= 0) return ps;
    if (k == m) return ps;
    if (k->w > r) break;
    r -= k->w;
    ps += k->p;
    if (k->x != NULL) *(k->x) = 1.0;
  }
  
  if (k->x != NULL) *(k->x) = r / (double) k->w;
  
  return ps + r * k->e;
  
}

double lpsolve (lpitem *a, const int c, const int n) {
  lpitem *k, *m;
  register int r, i;
  double ps;
  
  if (c <= 0) return 0;
  m = a + n;
  
  sort (a, m-1);
  
  for (i = 0, k = a, ps = 0, r = c; i<n; k++, ++i) {
    if (k->p <= 0) return ps;
    if (k == m) return ps; 
    if (k->w > r) break; 
    r -= k->w; 
    ps += k->p; 
    if (k->x == NULL) {
      printf("erro : x apontando para null \n");
      exit (EXIT_FAILURE);
    }
    *(k->x) = 1.0;
  }
  
  if(i < n) {
    if (k->x == NULL) {
      printf("erro : x apontando para null \n");
      exit (EXIT_FAILURE);
    } 
    *(k->x) = r / (double) k->w;
    ps += (double) r * k->e;
  }
  
  return ps ;
  
}

void add_proby (lpitem *a, const int n, const int c, double **p) {
  register int i, j;
  
  for (i = 0; i < n-1; i++)  {
      for (j = i+1; j < n; j++) {
	if (p[i][j] >= 0.0) {
	  a[i].p += p[i][j]/(double)2.0;
	  a[j].p += p[i][j]/(double)2.0;
	}
      }
  }
}

void sol_y (const int n, double **y, double **p, const double *xd) {
  register int i, j;
  
  for (i = 0; i < n-1; i++) {
    for (j = i+1; j < n; j++)	{
      if (p[i][j] >= 0.0) {
	y[i][j] = (xd[i] + xd[j])/2.0;
      } else y[i][j] = 0.0;
    }
  }
}
