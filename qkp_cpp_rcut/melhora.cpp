void melhora (const int n, int **p, const int *w, const int c, int *xprime, int *lb) {
  register int i, j;
  int gaini, gainj;
  int tot, gain, bgain, res;
  int *q;
  
  q = malloc_vetor_int(n);
  
  res = c;
  /*printf("improve: n %d res %d c %d\n", n, res, c);*/
  for (i = 0; i < n; i++) 
    if (xprime[i]) res -= w[i];
  
  for (;;) {
    for (i = 0; i < n; i++) {
      for (tot = p[i][i], j = 0; j < n; j++) {
	if ((j != i) && (xprime[j] != 0)) 
	  tot += p[i][j] + p[j][i];
      }
      q[i] = tot;
    }
    
    bgain = gaini = gainj = 0;
    for (i = 0; i < n; i++) {
      if (xprime[i] == 0) {
	if (w[i] <= res) {
	  gain = q[i];
	  if (gain > bgain) {
	    bgain = gain; gaini = i; gainj = -1;
	  }
	} else {
	  for (j = 0; j < n; j++) {
	    if (j == i) continue;
	    if (xprime[j] == 0) continue;
	    if (w[i] - w[j] <= res) {
	      gain = q[i] - q[j] - (p[i][j] + p[j][i]);
	      if (gain > bgain) {
		bgain = gain; gaini = i; gainj = j;
	      }
	    }
	  }
	}
      }
    }

    /*printf("best gain %d i %d j %d res %d\n", bgain, gaini, gainj, res); */
    if (bgain == 0) break;
    xprime[gaini] = 1;
    if (gainj != -1) xprime[gainj] = 0;
    if (gainj != -1) res += w[gainj] - w[gaini];
    else res -= w[gaini];
    if (res < 0) {
      printf("error\n"); exit(-1);
    }
  } /* end of while loop */
  
  for (gain = 0, res = c, i = 0; i < n; i++) {
    if (xprime[i] == 0) continue;
    res -= w[i];
    for (j = 0; j < n; j++) {
      if (xprime[j]) gain += p[i][j];
    }
  }
  
  if (res < 0) { printf("error \n"); exit(-1); }
  /*printf("possible solution %d\n", gain);*/
  
  if (gain > *lb) {
    /* printf("improved solution %d to %d\n", z+fixp, gain+fixp); */
    *lb = gain;
    for (i = 0; i < n; i++) xstar[i] = xprime[i];
  }
  
  free(q); q = NULL;
  
}
