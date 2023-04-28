void modelo_cover(XPRSprob prob_cover)
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
  
  z.aloca(ncol);
  qrtype.aloca(nrow);
  for(i=0; i<nrow; ++i) qrtype[i] = 'G';
  
  rhs.aloca(nrow);
  
  rhs[0] = C+1;
  
  obj.aloca(ncol);
  
  for(k=0; k<N; ++k) obj[k] = 0;
  
  mstart.aloca(ncol+1);
  
  mstart[0] = 0;
  
  i = N;

  mrwind.aloca(i);
  dmatval.aloca(i);
  
  for(i=0, q=0; i<N; ++i) // z_i
    {
      mrwind[q] = 0; // Mcover
      dmatval[q] = W[i];
      
      mstart[i+1] = ++q;
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
  
  XPRSloadglobal(prob_cover, "cover", ncol, nrow, &qrtype, &rhs, NULL, &obj, &mstart, NULL, &mrwind, &dmatval, &dlb, &dub, ngents, nsets, &qgtype, &mgcols, NULL, NULL, NULL, NULL, NULL);
  
  XPRSgetintattrib(prob_cover,XPRS_NAMELENGTH,&nl);
  if(ncol > nrow)
    cnames.aloca(ncol*(nl*8+1));
  else
    cnames.aloca(nrow*(nl*8+1));
  
  for(i=0, k=0; i<N; ++i)
    {
      strcpy(var, "z_");
      strcat(var, itoa(i+1));
      strcpy(&cnames[k], var);
      k += strlen(var)+1;
    }
  
  XPRSaddnames(prob_cover, 2, &cnames, 0, ncol-1);  

  k = 0;
  strcpy(var, "Mcover");
  strcpy(&cnames[k], var);
  k += strlen(var)+1;
  
  XPRSaddnames(prob_cover, 1, &cnames, 0, nrow-1);  
  
  if(pr_lp)
    XPRSwriteprob(prob_cover,"knapsack_cover","l");

}
