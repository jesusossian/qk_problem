void modelo_lift(XPRSprob prob_lift)
{
  int i, j, k, q, w;
  int ncol, nrow, ngents, nsets, nl;
  Array<char> qrtype, qgtype, cnames;
  char var[64];
  Array<int> mstart, mrwind, mgcols;
  Array<double> rhs, obj, dmatval, dlb, dub;
  
  ncol = N;
  nrow = 1;
  ngents = ncol;
  nsets = 0;
  
  xl.aloca(ncol);
  
  qrtype.aloca(nrow);
  
  for(i=0; i<nrow; ++i) qrtype[i] = 'L';
  
  rhs.aloca(nrow);
  
  rhs[0] = 0;
  
  obj.aloca(ncol);
  
  for(k=0, i=0; k<N; ++k)
    {
      if(z[k] <= 1-0.0001)
	{
	  obj[k] = 0;
	}
      else
	{
	  obj[k] = 1;
	  ++i;
	}
    }
  
  mstart.aloca(ncol+1);
  
  mstart[0] = 0;
  
  mrwind.aloca(i);
  dmatval.aloca(i);
  
  for(i=0, q=0; i<N; ++i) // xl_i
    {
      if(z[i] > 1 - 0.0001)
	{
	  mrwind[q] = 0; // Mlift
	  dmatval[q] = W[i];
	  ++q;
	}
      mstart[i+1] = q;
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
  
  XPRSloadglobal(prob_lift, "lift", ncol, nrow, &qrtype, &rhs, NULL, &obj, &mstart, NULL, &mrwind, &dmatval, &dlb, &dub, ngents, nsets, &qgtype, &mgcols, NULL, NULL, NULL, NULL, NULL);
  
  XPRSchgobjsense(prob_lift, XPRS_OBJ_MAXIMIZE);
  
  XPRSgetintattrib(prob_lift,XPRS_NAMELENGTH,&nl);
  if(ncol > nrow)
    cnames.aloca(ncol*(nl*8+1));
  else
    cnames.aloca(nrow*(nl*8+1));
  
  for(i=0, k=0; i<N; ++i)
    {
      strcpy(var, "xl_");
      strcat(var, itoa(i+1));
      strcpy(&cnames[k], var);
      k += strlen(var)+1;
    }
  
  XPRSaddnames(prob_lift, 2, &cnames, 0, ncol-1);  
  
  k = 0;
  strcpy(var, "Mlift");
  strcpy(&cnames[k], var);
  k += strlen(var)+1;
  
  XPRSaddnames(prob_lift, 1, &cnames, 0, nrow-1);  
  
  if(pr_lp)
    XPRSwriteprob(prob_lift,"knapsack_lift","l");

}
