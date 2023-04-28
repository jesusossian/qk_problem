void modelo(XPRSprob prob)
{
  int i, j, k, z, w, wq;
  int ncol, nrow, ngents, nsets, nl;
  char var[64];
  Array<char> qrtype, qgtype, cnames;
  Array<int> mstart, mrwind, mgcols;
  Array<double> rhs, obj, dmatval, dlb, dub;
  int newrow, nz, ad, l;
  
  if(eq_mochquad == 1)
    wq = N;
  else
    wq = 0;
  
  ncol = N*(N+1)/2;
  nrow = 1 + N*(N-1) + wq;
  if(eq_ln == 1) nrow += N*(N-1)/2;
  ngents = ncol;
  nsets = 0;
  
  //printf("Ncol: %d Nrow: %d ngents: %d eq_ln: %d nsets: %d \n" ,ncol, nrow, ngents, eq_ln, nsets);
  
  x.aloca(ncol);
  xz.aloca(ncol);
  
  cx.aloca(ncol);
  
  xp.aloca(N);
  
  qrtype.aloca(nrow);
  
  for(i=0; i<nrow; ++i) qrtype[i] = 'L';
  
  rhs.aloca(nrow);
  
  //printf("modelo - Lado direito\n");
  rhs[0] = C;
  j = N*(N-1) + wq;
  for(i=1; i<=j; ++i) rhs[i] = 0;
  if(eq_ln == 1)
    for(i=j+1; i<nrow; ++i) rhs[i] = 1;
  
  //printf("modelo - Funcao objetivo\n");
  obj.aloca(ncol);
  
  for(k=0; k<N; ++k) obj[k] = P[k];
  for(i=0; i<N-1; ++i)
    for(j=0; j<N-i-1; ++j, ++k)
      obj[k] = 2*PIJ[i][j];
  
  //printf("modelo - criando matriz\n");
  mstart.aloca(ncol+1);
  
  mstart[0] = 0;
  
  i = N + N*wq + 2*N*(N-1);
  if(eq_ln == 1) i += 3*N*(N-1)/2;
  //printf("i: %d\n",i);
  mrwind.aloca(i);
  dmatval.aloca(i);
  
  for(i=0, z=0; i<N; ++i) // x_i
    {
      mrwind[z] = 0; // MC
      dmatval[z] = W[i];
      
      if(eq_mochquad == 1)
	{
	  mrwind[++z] = i+1; //MCQ
	  dmatval[z] = -C + W[i];
	}
      
      for(j=0, k=0; j<i; ++j)
	{
	  mrwind[++z] = wq + N*(N-1)/2 + k + i-j; //Link
	  dmatval[z] = -1;
	  
	  if(mrwind[z] < 0 || mrwind[z] >= nrow)
	    {
	      printf("Erro - mrwind[%d] invalido %d (a)\n", z, mrwind[z]);
	      exit(0);
	    }
	  
	  if(eq_ln == 1)
	    {
	      mrwind[++z] = wq + N*(N-1) + k +i-j; //Ln
	      dmatval[z] = 1;
	      
	      if(mrwind[z] < 0 || mrwind[z] >= nrow)
		{
		  printf("Erro - mrwind[%d] invalido %d (b)\n", z, mrwind[z]);
		  exit(0);
		}
	    }
	  k += N-j-1;
	} // for(j)
      
      for(j=i+1; j<N; ++j) 
	{
	  mrwind[++z] = wq + k + j - i; //Link
	  dmatval[z] = -1;
	  
	  if(mrwind[z] < 0 || mrwind[z] >= nrow)
	    {
	      printf("Erro - mrwind[%d] invalido %d (c)\n", z, mrwind[z]);
	      exit(0);
	    }
	  
	  if(eq_ln == 1)
	    {
	      mrwind[++z] = wq + N*(N-1) + k + j-i; //Ln
	      dmatval[z] = 1;
	      
	      if(mrwind[z] < 0 || mrwind[z] >= nrow)
		{
		  printf("Erro - mrwind[%d] invalido %d %d %d(d)\n", z, mrwind[z],i, j);
		  exit(0);
		}
	    }
	}
      
      mstart[i+1] = ++z;
    
    }// for(i)
  
  if(eq_ln == 1)
    i = 2*N*N -N + wq;
  else
    i = N + wq + N*(N-1);
  if(mstart[N] != i)
    {
      printf("Erro - mstart[N] nao bate %d %d (1)\n", mstart[N], i);
      exit(0);
    }
  
  for(i=0; i<mstart[N]; ++i)
    if(mrwind[i] < 0 || mrwind[i] >= nrow)
      {
	printf("Erro - mrwind[%d] invalido %d (1)\n", i, mrwind[i]);
	exit(0);
      }
  
  for(i=0, k=N; i<N-1; ++i)
    for(j=0; j<N-i-1; ++j, ++k) // y[i][j]
      {
	if(eq_mochquad == 1)
	  {
	    mrwind[z] = 1 + i; // MCQ
	    dmatval[z] = W[i+j+1];
	    
	    mrwind[++z] = 2 + j + i; // MCQ
	    dmatval[z] = W[i];
	  }
	
	if(eq_mochquad == 1) ++z;
	
	mrwind[z] = 1 + k - N + wq; // Link
	dmatval[z] = 1;
	
	mrwind[++z] = 1 + N*(N-1)/2 + k - N + wq; // Link
	dmatval[z] = 1;
	
	if(mrwind[z] < 0 || mrwind[z] >= nrow)
	  {
	    printf("Erro - mrwind[%d] invalido %d (3)\n", z, mrwind[z]);
	    exit(0);
	  }
	
	if(eq_ln == 1)
	  {
	    mrwind[++z] = 1 + N*(N-1) + k - N + wq; // Ln
	    dmatval[z] = -1;
	    
	    if(mrwind[z] < 0 || mrwind[z] >= nrow)
	      {
		printf("Erro - mrwind[%d] invalido %d (4)\n", z, mrwind[z]);
		exit(0);
	      }
	  }
	
	mstart[k+1] = ++z;
  
      }
  
  if(k != ncol)
    {
      printf("Erro - ncol nao bate %d %d \n", k, ncol);
      exit(0);
    }
  
  i = N + wq*N + 2*N*(N-1);
  if(eq_ln == 1) i += 3*N*(N-1)/2;
  if(mstart[ncol] != i)
    {
      printf("Erro - mstart[ncol] nao bate %d %d (2)\n", i, mstart[ncol]);
      exit(0);
    }
  
  for(i=0; i<mstart[ncol]; ++i)
    if(mrwind[i] < 0 || mrwind[i] >= nrow)
      {
	printf("Erro - mrwind[%d] invalido %d (2)\n", i, mrwind[i]);
	exit(0);
      }
  
  dlb.aloca(ncol);
  dub.aloca(ncol);
  
  for(i=0; i<ncol; ++i)
    {
      dlb[i] = 0;
      dub[i] = 1;
    }
  
  qgtype.aloca(ngents);
  mgcols.aloca(ngents);
  
  for(i=0; i<ngents; ++i)
    {
      qgtype[i] = 'B';
      mgcols[i] = i;
    }
  
  //printf("modelo - lendo modelo\n");
  if(XPRSloadglobal(prob, "qknapsck", ncol, nrow, &qrtype, &rhs, NULL, &obj, &mstart, NULL, &mrwind, &dmatval, &dlb, &dub, ngents, nsets, &qgtype, &mgcols, NULL, NULL, NULL, NULL, NULL)) exit(1);
  
  XPRSgetintattrib(prob,XPRS_NAMELENGTH,&nl);
  
  //printf("nl: %d \n", nl);
  
  if(ncol > nrow)
    cnames.aloca(ncol*((nl+1)*8+1));
  else
    cnames.aloca(nrow*((nl+1)*8+1));
  
  for(i=0, k=0; i<N; ++i)
    {
      strcpy(var, "x_");
      strcat(var, itoa(i+1));
      strcpy(&cnames[k], var);
      k += strlen(var) + 1;
    }
  
  for(i=0; i<N-1; ++i)
    for(j=0; j<N-i-1; ++j/*, ++k*/)
      {
	strcpy(var, "y_");
	strcat(var, itoa(i+1));
	strcat(var,"_");
	strcat(var, itoa(i+j+2));
	strcpy(&cnames[k], var);
	k += strlen(var)+1;
      }
  
  //printf("modelo - depois de alocar 6\n");
  XPRSaddnames(prob, 2, &cnames, 0, ncol-1); 
  //printf("modelo - depois de alocar 7\n"); 
  
  k = 0;
  strcpy(var, "MC");
  strcpy(&cnames[k], var);
  k += strlen(var)+1;
  
  if(eq_mochquad == 1)
    for(i=0; i<N; ++i)
      {
	strcpy(var, "MCQ_");
	strcat(var, itoa(i+1));
	strcpy(&cnames[k], var);
	k += strlen(var)+1;
      }
  
  for(i=0; i<N-1; ++i)
    for(j=0; j<N-i-1; ++j)
      {
	strcpy(var, "LkA_");
	strcat(var, itoa(i+1));
	strcat(var,"_");
	strcat(var, itoa(j+1));
	strcpy(&cnames[k], var);
	k += strlen(var)+1;
      }
  
  for(i=0; i<N-1; ++i)
    for(j=0; j<N-i-1; ++j)
      {
	strcpy(var, "LkB_");
	strcat(var, itoa(i+1));
	strcat(var,"_");
	strcat(var, itoa(j+1));
	strcpy(&cnames[k], var);
	k += strlen(var)+1;
      }
  
  if(eq_ln == 1)
    for(i=0; i<N-1; ++i)
      for(j=0; j<N-i-1; ++j)
	{
	  strcpy(var, "Ln_");
	  strcat(var, itoa(i+1));
	  strcat(var,"_");
	  strcat(var, itoa(j+i+2));
	  strcpy(&cnames[k], var);
	  k += strlen(var)+1;
	}
  
  //printf("modelo - depois de alocar 8\n");
  XPRSaddnames(prob, 1, &cnames, 0, nrow-1);  
  //printf("modelo - depois de alocar 9\n");
  
  if(eq_trig0 == 1)
    {
      newrow = N*(N-1)*(N-2)/2;
      nz = 2*N*(N-1)*(N-2);
      printf("Triangular 0 - Newrow: %d NewNonZero: %d \n", newrow, nz);
      qrtype.redefine(newrow, 'L');
      rhs.redefine(newrow, 0);
      mstart.aloca(newrow);
      mrwind.aloca(nz);
      dmatval.aloca(nz);
      
      for(i=0, z=0, w=0; i<N-2; ++i)
	for(j=i+1; j<N-1; ++j)
	  for(k=j+1; k<N; ++k)
	    {
	      mstart[z] = w;
	      
	      //-x_i
	      dmatval[w] = -1;
	      mrwind[w] = i;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (5)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      for(l=0, ad=N; l<i; ++l)
		ad += N-l-1;
	      
	      // + y_ij
	      dmatval[++w] = 1;
	      mrwind[w] = ad + j -i -1;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (5)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      // + y_ik
	      dmatval[++w] = 1;
	      mrwind[w] = ad + k -i -1;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (5)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      for(l=0, ad=N; l<j; ++l)
		ad += N-l-1;
	      
	      // - y_jk
	      dmatval[++w] = -1;
	      mrwind[w] = ad + k -j -1;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (5)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      //++w; ++z;
	      mstart[++z] = ++w;
	      
	      //-x_j
	      dmatval[w] = -1;
	      mrwind[w] = j;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (6)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      for(l=0, ad=N; l<i; ++l)
		ad += N-l-1;

	      // + y_ij
	      dmatval[++w] = 1;
	      mrwind[w] = ad + j -i -1;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (6)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      // - y_ik
	      dmatval[++w] = -1;
	      mrwind[w] = ad + k -i -1;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (6)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      for(l=0, ad=N; l<j; ++l)
		ad += N-l-1;
	      
	      // + y_jk
	      dmatval[++w] = 1;
	      mrwind[w] = ad + k -j -1;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (6)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      mstart[++z] = ++w;
	      
	      //-x_k
	      dmatval[w] = -1;
	      mrwind[w] = k;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (7)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      for(l=0, ad=N; l<i; ++l)
		ad += N-l-1;
	      
	      // - y_ij
	      dmatval[++w] = -1;
	      mrwind[w] = ad + j -i -1;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (7)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      // + y_ik
	      dmatval[++w] = 1;
	      mrwind[w] = ad + k -i -1;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (7)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      for(l=0, ad=N; l<j; ++l)
		ad += N-l-1;
	      
	      // + y_jk
	      dmatval[++w] = 1;
	      mrwind[w] = ad + k -j -1;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (7)\n", w, mrwind[w]);
		  exit(0);
		}
	      ++w; ++z;
	    }
      
      if(z != newrow)
	{
	  printf("Erro - newrow nao bate %d %d (1)\n", newrow, z);
	  exit(0);
	}
      
      if(w != nz)
	{
	  printf("Erro - nz nao bate %d %d (1)\n", nz, w);
	  exit(0);
	}
      
      if(XPRSaddrows(prob, newrow, nz, &qrtype, &rhs, NULL, &mstart, &mrwind, &dmatval)) exit(9);
      
      printf("Adicinou a restricao Triangular < 0 \n");
      
      cnames.aloca(newrow*((nl+1)*8+1));
      
      for(i=0, w=0; i<N-2; ++i)
	for(j=i+1; j<N-1; ++j)
	  for(k=j+1; k<N; ++k)
	    {
	      strcpy(var, "tr0_");
	      strcat(var, itoa(i+1));
	      strcat(var,"_");
	      strcat(var, itoa(j+1));
	      strcat(var,"_");
	      strcat(var, itoa(k+1));
	      strcpy(&cnames[w], var);
	      w += strlen(var)+1;
	      strcpy(var, "tr0_");
	      strcat(var, itoa(j+1));
	      strcat(var,"_");
	      strcat(var, itoa(i+1));
	      strcat(var,"_");
	      strcat(var, itoa(k+1));
	      strcpy(&cnames[w], var);
	      w += strlen(var)+1;
	      strcpy(var, "tr0_");
	      strcat(var, itoa(k+1));
	      strcat(var,"_");
	      strcat(var, itoa(i+1));
	      strcat(var,"_");
	      strcat(var, itoa(j+1));
	      strcpy(&cnames[w], var);
	      w += strlen(var)+1;
	    }
      
      //printf("modelo - depois de alocar 6\n");
      XPRSaddnames(prob, 1, &cnames, nrow, nrow + newrow -1); 
      
      nrow += newrow;
    }// if(eq_trig0 == 1)
  
  if(eq_trig1 == 1)
    {
      newrow = N*(N-1)*(N-2)/6;
      nz = N*(N-1)*(N-2);
      printf("Triangular 1 - Newrow: %d NewNonZero: %d \n", newrow, nz);
      qrtype.redefine(newrow, 'L');
      rhs.redefine(newrow, 1);
      mstart.aloca(newrow);
      mrwind.aloca(nz);
      dmatval.aloca(nz);
      
      for(i=0, z=0, w=0; i<N-2; ++i)
	for(j=i+1; j<N-1; ++j)
	  for(k=j+1; k<N; ++k)
	    {
	      mstart[z] = w;
	      
	      //x_i
	      dmatval[w] = 1;
	      mrwind[w] = i;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (8)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      //x_j
	      dmatval[++w] = 1;
	      mrwind[w] = j;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (8)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      //x_k
	      dmatval[++w] = 1;
	      mrwind[w] = k;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (8)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      for(l=0, ad=N; l<i; ++l)
		ad += N-l-1;
	      
	      // - y_ij
	      dmatval[++w] = -1;
	      mrwind[w] = ad + j -i -1;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (8)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      // - y_ik
	      dmatval[++w] = -1;
	      mrwind[w] = ad + k -i -1;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (8)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      for(l=0, ad=N; l<j; ++l)
		ad += N-l-1;
	      
	      // - y_jk
	      dmatval[++w] = -1;
	      mrwind[w] = ad + k -j -1;
	      if(mrwind[w] < 0 || mrwind[w] >= ncol)
		{
		  printf("Erro - mrwind[%d] invalido %d (5)\n", w, mrwind[w]);
		  exit(0);
		}
	      
	      ++w; ++z;
	    }
      
      if(z != newrow)
	{
	  printf("Erro - newrow nao bate %d %d (2)\n", newrow, z);
	  exit(0);
	}
      
      if(w != nz)
	{
	  printf("Erro - nz nao bate %d %d (2)\n", nz, w);
	  exit(0);
	}
      
      if(XPRSaddrows(prob, newrow, nz, &qrtype, &rhs, NULL, &mstart, &mrwind, &dmatval)) exit(9);
      
      printf("Adicinou a restricao Triangular < 1 \n");
      
      cnames.aloca(newrow*((nl+1)*8+1));
      
      for(i=0, w=0; i<N-2; ++i)
	for(j=i+1; j<N-1; ++j)
	  for(k=j+1; k<N; ++k)
	    {
	      strcpy(var, "tr1_");
	      strcat(var, itoa(i+1));
	      strcat(var,"_");
	      strcat(var, itoa(j+1));
	      strcat(var,"_");
	      strcat(var, itoa(k+1));
	      strcpy(&cnames[w], var);
	      w += strlen(var)+1;
	    }
      
      //printf("modelo - depois de alocar 7\n");
      XPRSaddnames(prob, 1, &cnames, nrow, nrow + newrow -1); 
    }// if(eq_trig == 1)
  
  if(pr_lp)
    XPRSwriteprob(prob,"qknapsck","l");

}
