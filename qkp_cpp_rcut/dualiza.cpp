void dualiza_lini (const int n, const int nrl, const double *x, double **y, const double *lagl, bool *sitl, int *svidl, const int nvidl) {
  register int i, j, t;
  
  t = 0;
  for (i = 0; i < n-1; i++) {
    for (j = i+1; j < n; j++) {
      if (sitl[t] == false) {
	if (y[i][j] - x[i] > EPSILON) {
	  sitl[t] = true;
	  svidl[t] = 0;
	}
      }	else if (sitl[t] == true) {
	if ((y[i][j] - x[i] <= EPSILON) && (lagl[t] < EPSILON)) {
	  if (svidl[t] >= nvidl) {
	    sitl[t] = false;
	    svidl[t] = 0;
	  } else {
	    sitl[t] = true;
	    svidl[t] += 1;
	  }
	}
      }
      t++;
    }
  }
  
  if (t != nrl) {
    printf("error : numero de restricoes \n");
    exit(-1);
  }     
  
}

void dualiza_linj (const int n, const int nrl, const double *x, double **y, const double *lagl, bool *sitl, int *svidl, const int nvidl) {
  register int i, j, t;
  
  t = 0;
  for (i = 0; i < n-1; i++) {
    for(j = i+1; j < n; j++) {
      if (sitl[t] == false) {
	if (y[i][j] - x[j] > EPSILON) {
	  sitl[t] = true;
	  svidl[t] = 0;
	}
      } else if (sitl[t] == true) {
	if ((y[i][j] - x[j] <= EPSILON) && (lagl[t] < EPSILON)) {
	  if (svidl[t] >= nvidl) {
	    sitl[t] = false;
	    svidl[t] = 0;
	  } else {
	    sitl[t] = true;
	    svidl[t] += 1;
	  }
	}
      }
      t++;
    }
  }
  
  if (t != nrl) {
    printf("erro : numero de restricoes \n");
    exit(-1);
  }
  
}

void dualiza_linij (const int n, const int nrl, const double *x, double **y, const double *lagl, bool *sitl, int *svidl, const int nvidl) {
  register int i, j, t;
  
  t = 0;
  for (i = 0; i < n-1; i++) {
    for (j = i+1; j < n; j++) {
      if (sitl[t] == false) {
	if (x[i] + x[j] - y[i][j] > 1.0+EPSILON) {
	  sitl[t] = true;
	  svidl[t] = 0;
	}
      } else if (sitl[t] == true) {
	if ((x[i] + x[j] - y[i][j] <= 1 + EPSILON) && (lagl[t] < EPSILON)) {
	  if (svidl[t] >= nvidl) {
	    sitl[t] = false;
	    svidl[t] = 0;
	  } else {
	    sitl[t] = true;
	    svidl[t] += 1;
	  }
	}
      }
      t++;
    }
  }
  
  if (t != nrl) {
    printf("erro : numero de restricoes \n");
    exit(-1);
  }
  
}
