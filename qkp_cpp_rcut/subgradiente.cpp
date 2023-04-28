void subgradiente (int n, int **p, int *w, int c, double *ubmin, int *lbmax) {
  register int i, j;
  int it;
  double step;
  int ItNoImprov;
  double ub, ubfixa;
  int lb;
  double pi;
  int MaxIt;
  int MaxItNoImprov;
  double fpasso;
  double epsilon;
  double qgrad;
  int fgrad_mq;
  double slag;
  double ubp;
  int nrl;
  int heur_ite;

  double *lagmq, *lagli, *laglj, *laglij;
  double *gradmq, *gradli, *gradlj, *gradlij;

  double **pp;
  bool *sitli, *sitlj, *sitlij;

  int *xp;
  double *xd;
  double **y;

  int nvidal;
  int *svidali, *svidalj, *svidalij;
  int melhora_ub;

  bool status;
  int repetesol = 0;

  int *vetori;
  int *vetorw;
  int *vetoriw;
  int somai, somaw, somaiw, snew;

  lpitem *k;
  lpitem *a;

  step = 0;
  ItNoImprov = 0;
  *ubmin = INFTY;
  ub = 0.0;
  lb = 0;
  pi = 2;
  MaxIt = 3000+n;
  MaxItNoImprov = 90;
  fpasso = 0.6;
  epsilon = 0.005;
  lagbest = 0;
  ubp = 0.0;
  nvidal = 2;
  snew = 0;
  heur_ite = 1500+n;

  vetori = calloc_vetor_int(n*(3000+n));
  vetorw =calloc_vetor_int(n*(3000+n));
  vetoriw =calloc_vetor_int(n*(3000+n));

  a = (lpitem *) malloc(n * sizeof(lpitem));
  if (a == NULL) {
    printf ("erro: memoria insuficiente \n");
    exit (EXIT_FAILURE);
  }
  
  for (i = 0; i < n; i++) {
    for (j = i; j < n; j++) {
      pbest[i][j] = p[i][j];
    }
  }
  
  nrl = n*(n-1)/2;
  
  xd = calloc_vetor_double(n);
  xp = calloc_vetor_int(n);
  y = calloc_matriz_double(n,n);
  pp = malloc_matriz_double(n,n);

  for (i = 0; i < n; i++) {
    for (j = i; j < n; j++) pp[i][j] = p[i][j];
  }
  
  gradmq = calloc_vetor_double(n);
  gradli = calloc_vetor_double(nrl);
  gradlj = calloc_vetor_double(nrl);
  gradlij = calloc_vetor_double(nrl);
  
  lagmq = calloc_vetor_double(n);
  lagli = calloc_vetor_double(nrl);
  laglj = calloc_vetor_double(nrl);
  laglij = calloc_vetor_double(nrl);  
  
  sitli = malloc_vetor_bool(nrl);
  sitlj = malloc_vetor_bool(nrl);
  sitlij = malloc_vetor_bool(nrl);
  for (i = 0; i < nrl; i++) {
    sitli[i] = false;
    sitlj[i] = false;
    sitlij[i] = false;
  }
    
  svidali = calloc_vetor_int(nrl);
  svidalj = calloc_vetor_int(nrl);
  svidalij = calloc_vetor_int(nrl);
  
  fgrad_mq = 0;
  for (i = 0; i < n; i++) {
    fgrad_mq += w[i];
  }
  fgrad_mq += c;

  for (it = 1; it < MaxIt; it++) {            
    /*calculo dos multiplicadores de lagrange*/
    lagrange (n, gradmq, lagmq, step);
    lagrange (nrl, gradli, lagli, step);
    lagrange (nrl, gradlj, laglj, step);
    lagrange (nrl, gradlij, laglij, step);
    
    for (i = 0; i < n; i++) {
      for (j = i; j < n; j++) {
	pp[i][j] = p[i][j];
      }
    }
    
    /*atualiza custos lagrangeano*/
    custolag_mq (n, w, c, pp, lagmq);
    custolag_lini (n, nrl, pp, lagli);
    custolag_linj (n, nrl, pp, laglj);
    custolag_linij (n, nrl, pp, laglij);
    
    /*resolve problema da mochila*/
    for (i = 0, k = &(a[0]); i < n; i++, k++) {
      k->p = pp[i][i];
      k->w = w[i];
      k->x = &xd[i];
      xd[i] = 0.0;
    }
    
    add_proby(a, n, c, pp);
    
    for (i = 0, k = &(a[0]); i < n; i++, k++) 
      k->e = k->p/(double)k->w;
    
    ub = lpsolve (a, c, n);    
    sol_y(n, y, pp, xd);
    
    slag = 0.0;
    
    for (i = 0; i < nrl; i++) {
      ub += laglij[i];
      slag += laglij[i];
    }
    //ub = (int)ub;
    
    /*dualiza restricoes*/
    dualiza_lini (n, nrl, xd, y, lagli, sitli, svidali, nvidal);
    dualiza_linj (n, nrl, xd, y, laglj, sitlj, svidalj, nvidal);
    dualiza_linij (n, nrl , xd, y, laglij, sitlij, svidalij, nvidal);
    
    /*calculo do gradiente e quadrado do gradiente*/
    gradiente_mq (n, w, c, xd, y, gradmq, fgrad_mq);
    gradiente_lini (n, nrl, xd, y, gradli, sitli);
    gradiente_linj (n, nrl, xd, y, gradlj, sitlj);
    gradiente_linij (n, nrl , xd, y, gradlij, sitlij);
    
    /*calculo do quadrado do gradiente*/
    qgrad = 0.0;
    qgrad += quadgrad_mq (n, gradmq);
    qgrad += quadgrad (nrl, gradli, sitli);
    qgrad += quadgrad (nrl, gradlj, sitlj);
    qgrad += quadgrad (nrl, gradlij, sitlij);
    
    melhora_ub = 0;
    if (ub < *ubmin) {
      melhora_ub = 1;
      *ubmin = ub;
      ItNoImprov = 0;
      lagbest = slag;
      
      printf("it = %d, lb = %d , ub = %.2f, pi = %.5f \n", it, *lbmax, *ubmin, pi);
      
      for (i = 0; i < n; i++) {
	for (j = i; j < n; j++) {
	  pbest[i][j] = pp[i][j];
	}
      }
      
      if (opc.hbillionnet == 1) {
	for (i = 0; i < n; i++) xp[i] = (int)xd[i];
	melhora(n, p, w, c, xp, lbmax);
      }
      
    } else {
      ItNoImprov++;
      if (ItNoImprov == MaxItNoImprov) {
	pi *= fpasso;
	ItNoImprov = 0;
	if (pi < EPSILON) {
	  //printf("passo pequeno \n");
	  break;
	}
      }
    }
    
    if ((opc.guloso_bl == 1) && (it < heur_ite)) {
      somai = somaw = somaiw = 0;
      for (i = 0; i < n; i++) {
	xp[i] = (int)xd[i];
	if (xp[i] == 1) {
	  somai += 1;
	  somaw += w[i];
	  somaiw += i*w[i];
	}
      }
      
      if (snew == 0) {
	snew += 1;
	vetori[snew] = somai;
	vetorw[snew] = somaw;
	vetoriw[snew] = somaiw;
	melhora(n, p, w, c, xp, lbmax); 
      } else {
	status = false;
	for (i = 1; i <= snew; i++) {
	  if ((vetori[i] == somai) && (vetorw[i] == somaw) && (vetoriw[i] == somaiw)) {
	    status = true;
	    repetesol++;
	    break;
	  }
	}
	
	if(status == false) {
	  snew += 1;
	  vetori[snew] = somai;
	  vetorw[snew] = somaw;
	  vetoriw[snew] = somaiw;
	  melhora(n, p, w, c, xp, lbmax);
	}
      }  
      
    }
      
    if (*ubmin - (double)(*lbmax) <= 1 - EPSILON) {
      //printf ("A solução viável é otima! \n");
      break;
    }
    
    if (qgrad < EPSILON) {
      //printf ("Nenhuma restrição violada! \n");
      break;
    }
    
    if ( (*ubmin - *lbmax) / *lbmax < 0.01)
	ubp = *ubmin * 1.01;
      else
	ubp = *ubmin;
    
    step = pi * (ubp - *lbmax) / qgrad;
    
  }/*fim do subgradiente*/
  
  nite = it;
  
  free(lagmq);lagmq = NULL;
  free(lagli); lagli = NULL;
  free(laglj); laglj = NULL;
  free(laglij); laglij = NULL;
  
  free(gradmq); gradmq = NULL;
  free(gradli); gradli = NULL;
  free(gradlj); gradlj = NULL;
  free(gradlij); gradlij = NULL;
  
  for (i = 0; i < n; i++) {
    free(pp[i]);
    pp[i] = NULL;
  }
  free(pp); pp = NULL;
  
  free(sitli); sitli = NULL;
  free(sitlj); sitlj = NULL;
  free(sitlij); sitlij = NULL;
  
  free(xp); xp = NULL;
  
  free(xd); xd = NULL;
  
  for (i = 0; i < n; i++) {
    free(y[i]);
    y[i] = NULL;
  }
  free(y); y = NULL;
  
  free(svidali); svidali = NULL;
  free(svidalj); svidalj = NULL;
  free(svidalij); svidalij = NULL;
  
  free (a); a = NULL;
  
  free(vetori); vetori= NULL;
  free(vetorw); vetorw= NULL;
  free(vetoriw); vetoriw= NULL;
  
}
