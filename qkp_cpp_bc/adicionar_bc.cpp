int adicionar_bc_cover(XPRSprob prob, Array<int> &mtype, Array<char> &qrtype, Array<double> &drhs, Array<int> &mstart, double objval_cover, Array<int> &mcols, Array<double> &dmatval)
{
  int i, card_cover, ncut;
  
  ncut = 0;
  card_cover = 0;
  
  for(i=0; i<N;++i) 
    {
      if(z[i] > 1-0.0001) 
	{
	  dmatval[ card_cover ] = 1;
	  mcols[ card_cover ] = i;
	  ++card_cover;
	}
    }
  
  drhs[0] = card_cover-1;
  mstart[1] = card_cover;
  
  ncut++;
  
  if(ncut > 0)
    if(XPRSaddcuts(prob, ncut, &mtype, &qrtype, &drhs, &mstart, &mcols, &dmatval)) exit(9);

  return ncut;
}

int adicionar_bc_cover_quad(XPRSprob prob, Array<int> &mtype, Array<char> &qrtype, Array<double> &drhs, Array<int> &mstart, double objval_cover, Array<int> &mcols, Array<double> &dmatval)
{
  int i, j, k, card_cover;
  float left_cover;
  Array< Array<double> > y;
  int w, p, ncut;
  int newrow, nz, ad, l;
  
  y.aloca(N);
  for(i=0;i<N;++i)
    y[i].redefine(N,0);
  
  k=N;
  for(i=0; i<N; ++i) 
    {
      for(j=0; j<N-i-1; ++j, ++k) 
	{
	  y[i][i+j+1] = x[k];
	}
    }
  
  newrow = N;
  nz = 2*N*(N-1)*(N-2);
  qrtype.redefine(newrow,'L');
  drhs.redefine(newrow,0);
  mstart.aloca(newrow+1);
  mcols.aloca(nz);
  dmatval.aloca(nz);
  
  ncut = 0;
  card_cover = 0;
  
  for(i=0; i<N; ++i)
    {
      if(z[i] > 0.0001) ++card_cover;
    }
  
  mstart[0] = 0;
  for(i=0, p=0, w=0; i<N; ++i)
    {
      left_cover = 0;
      if(x[i] > 0.0001)
	{
	  for(j=0; j<N; ++j)
	    {
	      if(z[j] > 1-0.0001)
		{
		  if(i<j) left_cover += y[i][j];
		  if(i>j) left_cover += y[j][i];
		}
	    }
	  if(z[i] > 1-0.0001)
	    {
	      left_cover += (2-card_cover)*x[i];
	    } 
	  else
	    {
	      left_cover += (1-card_cover)*x[i];
	    }
	  
	  if (left_cover > 0.0001)
	    {
	      if(z[i] > 1-0.0001)
		{
		  dmatval[w] = 2-card_cover;
		  mcols[w] = i;
		}
	      else
		{
		  dmatval[w] = 1-card_cover;
		  mcols[w] = i;
		}
	      
	      for(j=0; j<N; ++j)
		{
		  if(z[j] >1-0.0001)
		    {
		      if(i<j)
			{
			  for(l=0, ad=N; l<i; ++l) ad += N-l-1;
			  
			  dmatval[++w] = 1;
			  mcols[w] = ad + j - i - 1;
			}
		      if(i>j)
			{
			  for(l=0, ad=N; l<j; ++l) ad += N-l-1;
			  
			  dmatval[++w] = 1;
			  mcols[w] = ad + i - j - 1;
			}
		    }
		}
	      
	      mstart[++p] = ++w;
	      ncut++;
	    }
	}
  }
  
  if(ncut > 0)
    if(XPRSaddcuts(prob, ncut, &mtype, &qrtype, &drhs, &mstart, &mcols, &dmatval)) exit(9);
  
  return ncut;
}

int adicionar_bc_extender(XPRSprob prob, Array<int> &mtype, Array<char> &qrtype, Array<double> &drhs, Array<int> &mstart, double objval_cover, Array<int> &mcols, Array<double> &dmatval)
{
  int i, card_cover, card_ext, max, ncut;
  
  card_cover = 0;
  card_ext = 0;
  max = 0;
  ncut = 0;

  for(i=0; i<N;++i) 
    {
      if(z[i] > 1-0.0001)
	{
	  dmatval[ card_cover ] = 1;
	  mcols[ card_cover ] = i;
	  ++card_cover;
	  if(W[i] >= max) max = W[i];
	}
    }
  
  card_ext = card_cover;
  
  for(i=0;i<N;++i) 
    {
      if((z[i] < 0.0001)&&(W[i] >= max)) 
	{
	  dmatval[ card_ext ] = 1;
	  mcols[ card_ext ] = i;
	  ++card_ext;
	}
    }
  
  drhs[0] = card_cover-1;
  mstart[1] = card_ext;
  
  ncut++;
  
  if(ncut > 0)
    if(XPRSaddcuts(prob, ncut, &mtype, &qrtype, &drhs, &mstart, &mcols, &dmatval)) exit(9);

  return ncut;

}

int adicionar_bc_extender_quad(XPRSprob prob, Array<int> &mtype, Array<char> &qrtype, Array<double> &drhs, Array<int> &mstart, double objval_cover, Array<int> &mcols, Array<double> &dmatval)
{
  
  int i, j, k, card_cover, card_ext, max, ncut;
  float left_ext;
  Array< Array<double> > y;
  int w, p, nr, np, ad, l;

  y.aloca(N);
  for(i=0; i<N; ++i)
    y[i].redefine(N,0);

  k=N;
  for(i=0; i<N; ++i) 
    {
      for(j=0; j<N-i-1; ++j, ++k) 
	{
	  y[i][i+j+1] = x[k];
	}
    }
  
  card_cover = 0;
  card_ext = 0;
  max = 0;
  ncut = 0;
  
  nr = N;
  np = 2*N*(N-1)*(N-2);
  qrtype.redefine(nr,'L');
  drhs.redefine(nr,0);
  mstart.aloca(nr+1);
  mcols.aloca(np);
  dmatval.aloca(np);
  
  for(i=0; i<N; ++i) 
    {
      if(z[i] > 1-0.0001) 
	{
	  ++card_cover;
	  if(W[i] >= max) max = W[i];
	}
    }
  card_ext = card_cover;

  for(i=0; i<N; ++i) 
    {
      if((z[i] < 0.0001) && (W[i] >= max)) 
	{
	  ++card_ext;
	}
    }
  
  mstart[0] = 0;
  for(i=0, p=0, w=0; i<N; ++i) 
    {
      left_ext = 0;
      if(x[i] > 0.0001) 
	{
	  for(j=0; j<N; ++j) 
	    {
	      if((z[j] > 1-0.0001) || ((z[j] < 0.0001) && (W[j] >= max))) 
		{
		  if(i < j) left_ext += y[i][j];
		  if(i > j) left_ext += y[j][i];
		}
	    }
	  
	  if((z[i] > 1-0.0001) || ((z[i] < 0.0001) && (W[i] >= max))) 
	    {
	      left_ext += (2-card_cover)*x[i];
	    } 
	  else 
	    {
	      left_ext += (1-card_cover)*x[i];
	    }
	  
	  if(left_ext > 0.0001) 
	    {
	      if((z[i]>1-0.0001) || ((z[i]<0.0001) && (W[i] >= max))) 
		{
		  dmatval[w] = 2-card_cover;
		  mcols[w] = i;
		} 
	      else 
		{
		  dmatval[w] = 1-card_cover;
		  mcols[w] = i;
		}
	      
	      for(j=0; j<N; ++j) 
		{
		  if((z[j] > 0.0001) || ((z[j] < 0.0001) && (W[j] >= max))) 
		    {
		      if(i < j) 
			{
			  for(l=0, ad=N; l<i; ++l) 
			    ad += N-l-1;
			  
			  dmatval[++w] = 1;
			  mcols[w] = ad+j-i-1;
			}
		      if(i > j) 
			{
			  for(l=0, ad=N; l<j; ++l) 
			    ad += N-l-1;
			  
			  dmatval[++w] = 1;
			  mcols[w] = ad+i-j-1;
			}
		    }
		}
	      mstart[++p] = ++w;
	      ncut++;
	    }
	}
    }
  
  if(ncut > 0)
    if(XPRSaddcuts(prob, ncut, &mtype, &qrtype, &drhs, &mstart, &mcols, &dmatval)) exit(9);
  
  return ncut;

}

int adicionar_bc_lift(XPRSprob prob, Array<int> &mtype, Array<char> &qrtype, Array<double> &drhs, Array<int> &mstart, double objval_cover, Array<int> &ncols, Array<double> &dmatval, pxpress * probl)
{
  int i, card_cover, NC, t, q, ncut, card_lift, add_lift, nl, k;
  double objval_lift;
  Array<int> lmindex, lmstart(N), lncols(1);
  Array<double> lobj(N), lrhs, ldmatval(N);
  char var[64];
  Array<char> cnames;
  
  Array<int> pd(N);
  Array<int> wd(N);
  Array<int> aux(N);
  Array<int> wl(N);
  Array<double> alpha;
  
  int bd;
  
  card_cover = 0;
  ncut = 0;
  
  if(XPRScreateprob(&probl->lift)) exit(9);
  XPRSsetlogfile(probl->lift, "loglift.txt");  
  modelo_lift(probl->lift);	  
  
  for(i=0, k=0, NC=0; i<N; ++i) 
    { 
      if(z[i] <= 1-0.0001) 
	{
	  wl[NC] = W[i];
	  aux[NC] = i;
	  ++NC;
	} 
      else 
	{
	  pd[k] = 1;
	  wd[k] = W[i];
	  ++card_cover;
	  ++k;
	}
    }
  
  ordena_d(&wl, 0, NC, &aux);
  
  lrhs.aloca(1);
  lmindex.aloca(1);
  alpha.aloca(N);
  
  for(i=0; i<NC; ++i) 
    {
      alpha[aux[i]] = 0;
    }
  
  for(t=0; t<NC; ++t) 
    {
      lrhs[0] = C-W[ aux[t] ];
      lmindex[0] = 0;
      if(XPRSchgrhs(probl->lift, 1, &lmindex, &lrhs)) exit(9);
      
      if(pr_lp)
	XPRSwriteprob(probl->lift, "modelo_lift", "l");
      
      XPRSmipoptimize(probl->lift, "");
      XPRSgetmipsol(probl->lift, &xl, NULL);
      XPRSgetdblattrib(probl->lift,XPRS_MIPOBJVAL, &objval_lift);
      
      lncols[0] = aux[t];
      alpha[aux[t]] = card_cover-1-objval_lift;
      
      if(alpha[aux[t]] < -0.0001)
	{
	  return 0;
	}
      
      if(alpha[aux[t]] < 0.0001) 
	{
	  break;
	}
      
      if(XPRSchgobj(probl->lift, 1, &lncols, &alpha)) exit(9);
      
      if(XPRSchgcoef(probl->lift, 0, aux[t], W[ aux[t] ])) exit(9);
    }
  
  card_lift = 0;
  for(i=0; i<N; ++i) 
    {
      if(z[i] > 1-0.0001) 
	{
	  dmatval[ card_lift ] = 1;
	  ncols[ card_lift ] = i;
	  ++card_lift;
	} 
      else if(alpha[i] > 0.0001)
	{
	  dmatval[ card_lift ] = alpha[i];
	  ncols[ card_lift ] = i;
	  ++card_lift;
	}
    }
  
  drhs[0] = card_cover-1;
  mstart[1] = card_lift;
  ncut++;
  
  if(ncut > 0)
    {
      if(XPRSaddcuts(prob, ncut, &mtype, &qrtype, &drhs, &mstart, &ncols, &dmatval)) exit(9);
    }
  
  return ncut;
}

int adicionar_bc_lift_quad(XPRSprob prob, Array<int> &mtype, Array<char> &qrtype, Array<double> &drhs, Array<int> &mstart, double objval_cover, Array<int> &mcols, Array<double> &dmatval, pxpress * probl)
{
  int i, card_cover, NC, t, q, ncut, card_lift, add_lift, nl, k, l, j;
  double objval_lift;
  double left_lift;
  Array<int> lmindex, lmstart(N), lncols(1);
  Array<double> lobj(1), lrhs, ldmatval(N);
  Array< Array<double> > y;
  char var[64];
  Array<char> cnames;
  int w, p, nr, np, ad;

  Array<int> wd;
  Array<int> pd;
  int bd;

  y.aloca(N-1);
  for(i=0; i<N-1; ++i)
    y[i].redefine(N-i-1,0);

  k=N;
  for(i=0; i<N-1; ++i) 
    {
      for(j=0; j<N-i-1; ++j, ++k) 
	{
	  y[i][j] = x[k];
	}
    }
  
  nr = N;
  np = 2*N*(N-1)*(N-2);
  mtype.redefine(nr, mtype[0]);
  qrtype.redefine(nr,'L');
  drhs.redefine(nr,0);
  mstart.aloca(nr+1);
  mcols.aloca(np);
  dmatval.aloca(np);

  Array<int> aux(N);
  Array<int> wl(N);
  Array<double> alpha;
  
  card_cover = 0;
  ncut = 0;
  
  if(XPRScreateprob(&probl->lift)) exit(9);
  
  modelo_lift(probl->lift);	  
  
  for(i=0, NC=0; i<N;++i) 
    { 
      if(z[i] < 1-0.0001) 
	{
	  wl[NC] = W[i];
	  aux[NC] = i;
	  ++NC;
	} 
      else 
	{
	  ++card_cover;
	}
    }

  ordena_d(&wl, 0, NC, &aux);

  lrhs.aloca(1);
  lmindex.aloca(1);
  alpha.aloca(N);
  
  for(i=0; i<NC; ++i) 
    {
      alpha[aux[i]] = 0;
    }
  
  for(t=0; t<NC; ++t) 
    {
      lrhs[0] = C-W[ aux[t] ];
      lmindex[0] = 0;
      if(XPRSchgrhs(probl->lift, 1, &lmindex, &lrhs)) exit(9);
      
      if(pr_lp) XPRSwriteprob(probl->lift, "modelo_lift", "l");
      
      XPRSmipoptimize(probl->lift, "");
      XPRSgetmipsol(probl->lift, &xl, NULL);
      XPRSgetdblattrib(probl->lift,XPRS_MIPOBJVAL, &objval_lift);
      
      lncols[0] = aux[t];
      alpha[aux[t]] = lobj[0] = card_cover-1- objval_lift;
      
      if(alpha[aux[t]] < 0.0001) 
	{
	  break;
	}
      
      if(XPRSchgobj(probl->lift, 1, &lncols, &lobj)) exit(9);
      
      if(XPRSchgcoef(probl->lift, 0, aux[t], W[ aux[t] ])) exit(9);
      
    }
  
  mstart[0] = 0;
  for(i=0, p=0, w=0; i<N; ++i)
    {
      left_lift = 0;
      if(x[i] > 0.0001)
	{
	  for(j=0; j<N; ++j)
	    {
	      if(z[j]>1-0.0001) 
		{
		  if(i<j) left_lift += y[i][j-i-1];
		  if(i>j) left_lift += y[j][i-j-1];
		}
	      else
		{
		  if(i<j) left_lift += alpha[j]*(y[i][j-i-1]);
		  if(i>j) left_lift += alpha[j]*(y[j][i-j-1]);
		}
	    }
	  
	  if(z[i]>1-0.0001) 
	    {
	      left_lift += (2-card_cover)*x[i];
	    }
	  else
	    {
	      left_lift += (alpha[i]+1-card_cover)*x[i];
	    }
	  
	  if(left_lift > 0.0001) 
	    {
	      if(z[i]>1-0.0001) 
		dmatval[w] = 2-card_cover; 
	      else
		dmatval[w] = alpha[i]+1-card_cover; 
	      
	      mcols[w] = i;
	      
	      for(j=0; j<N; ++j) 
		{
		  if(z[j]>1-0.0001)
		    {
		      if(i<j) 
			{
			  for(l=0, ad=N; l<i; ++l) ad += N-l-1;
			  
			  dmatval[++w] = 1; 
			  mcols[w] = ad+j-i-1;
			}
		      if(i>j)
			{
			  for(l=0, ad=N; l<j; ++l) ad += N-l-1;
			  
			  dmatval[++w] = 1; 
			  mcols[w] = ad+i-j-1; 
			}
		    }
		  else 
		    if(alpha[j] > 0.0001)
		      {
			if(i<j) 
			  {
			    for(l=0, ad=N; l<i; ++l) ad += N-l-1;
			    
			    dmatval[++w] = alpha[j]; 
			    mcols[w] = ad+j-i-1;
			  }
			if(i>j) 
			  {
			    for(l=0, ad=N; l<j; ++l) ad += N-l-1;
			    
			    dmatval[++w] = alpha[j]; 
			    mcols[w] = ad+i-j-1;
			  }
		      }
		}
	      
	      mstart[++p] = ++w;
	      ncut++;
	    }//end if(left_lift > 0.0001)
	}//end if(x[i] > 0.0001)
    }//end for(i = ..)
  
  if(ncut>0)
    {
      if(XPRSaddcuts(prob, ncut, &mtype, &qrtype, &drhs, &mstart, &mcols, &dmatval)) exit(9);
    }
  
  return ncut;
}


int adicionar_bc_tri(XPRSprob prob, Array<int> &mtype, Array<char> &qrtype, Array<double> &rhs, Array <int> &mstart, Array<int> &mrwind, Array<double> &dmatval)
{
  int i, j, k;
  int nrow, nl;
  char var[64];
  Array<char> cnames;
  Array<Array<double> > y;
  int w, z, ncut = 0, ncutt;

  y.aloca(N);
  for(i=0;i<N;++i)
    y[i].redefine(N, 0);
  
  k = N;
  for(i=0;i<N-1;++i) 
    {
      for(j=0;j<N-i-1;++j, ++k)
	{
	  y[i][i+j+1] = x[k];
	}
    }
  
  if(eq_trig0 == 2)
    {
      int newrow = N*(N-1)*(N-2)/2, nz = 2*N*(N-1)*(N-2), ad, l;
      qrtype.redefine(newrow, 'L');
      rhs.redefine(newrow, 0);
      mstart.aloca(newrow+1);
      mrwind.aloca(nz);
      dmatval.aloca(nz);
      ncutt = 0;
    
      mstart[0] = 0;
      for(i=0, z=0, w=0; i<N-2; ++i)
	for(j=i+1; j<N-1; ++j)
	  for(k=j+1; k<N; ++k)
	    {
	      if( -x[i] + y[i][j] +  y[i][k] - y[j][k] >= 0.0001) 
		{
		  //-x_i
		  dmatval[w] = -1;
		  mrwind[w] = i;
		  
		  for(l=0, ad=N; l<i; ++l)
		    ad += N-l-1;
		  
		  //+ y_ij
		  dmatval[++w] = 1;
		  mrwind[w] = ad + j -i -1;
		  
		  //+ y_ik
		  dmatval[++w] = 1;
		  mrwind[w] = ad + k -i -1;
		  
		  for(l=0, ad=N; l<j; ++l)
		    ad += N-l-1;
		  
		  //- y_jk
		  dmatval[++w] = -1;
		  mrwind[w] = ad + k -j -1;
		  
		  mstart[++z] = ++w;
		  ncut++; ncutt++;
		}
	      
	      if( -x[j] + y[i][j] -  y[i][k] + y[j][k] >= 0.0001)
		{
		  //-x_j
		  dmatval[w] = -1;
		  mrwind[w] = j;
		  
		  for(l=0, ad=N; l<i; ++l)
		    ad += N-l-1;
		  
		  //+ y_ij
		  dmatval[++w] = 1;
		  mrwind[w] = ad + j -i -1;
		  
		  //- y_ik
		  dmatval[++w] = -1;
		  mrwind[w] = ad + k -i -1;
		  
		  for(l=0, ad=N; l<j; ++l)
		    ad += N-l-1;
		  
		  //+ y_jk
		  dmatval[++w] = 1;
		  mrwind[w] = ad + k -j -1;
		  
		  mstart[++z] = ++w;

		  ncut++; ncutt++;
		}
	      
	      if( -x[k] - y[i][j] +  y[i][k] + y[j][k] >= 0.0001)
		{
		  //-x_k
		  dmatval[w] = -1;
		  mrwind[w] = k;
		  
		  for(l=0, ad=N; l<i; ++l)
		    ad += N-l-1;
		  
		  //- y_ij
		  dmatval[++w] = -1;
		  mrwind[w] = ad + j -i -1;
	   
		  //+ y_ik
		  dmatval[++w] = 1;
		  mrwind[w] = ad + k -i -1;
		  
		  for(l=0, ad=N; l<j; ++l)
		    ad += N-l-1;
		  
		  //+ y_jk
		  dmatval[++w] = 1;
		  mrwind[w] = ad + k -j -1;
		  
		  mstart[++z] = ++w;
		  ncutt++; 
		}
	    }
      
      //printf("# cortes add na restricao do triangulo < 0: %d\n", ncutt);
      
      if(ncutt > 0)
	if(XPRSaddcuts(prob, ncutt, &mtype, &qrtype, &rhs, &mstart, &mrwind, &dmatval)) exit(9);
      
      //printf("Adicinou a restricao Triangular < 0 \n");
      
      ncut = ncutt;
      
    }// if(eq_trig0 == 1)
  
  if(eq_trig1 == 2)
    {
      int newrow = N*(N-1)*(N-2)/6, nz = N*(N-1)*(N-2), ad, l;
      qrtype.redefine(newrow, 'L');
      rhs.redefine(newrow, 1);
      mstart.aloca(newrow+1);
      mrwind.aloca(nz);
      dmatval.aloca(nz);
      ncutt = 0;
      mstart[0] = 0;
      for(i=0, z=0, w=0; i<N-2; ++i)
	for(j=i+1; j<N-1; ++j)
	  for(k=j+1; k<N; ++k)
	    {
	      if( x[i] + x[j] + x[k] - y[i][j] - y[i][k] - y[j][k] >= 1+0.0001)
		{
		  //x_i
		  dmatval[w] = 1;
		  mrwind[w] = i;
		  
		  //x_j
		  dmatval[++w] = 1;
		  mrwind[w] = j;
		  
		  //x_k
		  dmatval[++w] = 1;
		  mrwind[w] = k;
		  
		  for(l=0, ad=N; l<i; ++l)
		    ad += N-l-1;
		  
		  //- y_ij
		  dmatval[++w] = -1;
		  mrwind[w] = ad + j -i -1;
		  
		  //- y_ik
		  dmatval[++w] = -1;
		  mrwind[w] = ad + k -i -1;
		  
		  for(l=0, ad=N; l<j; ++l)
		    ad += N-l-1;
		  
		  //- y_jk
		  dmatval[++w] = -1;
		  mrwind[w] = ad + k -j -1;
		  
		  mstart[++z] = ++w;
		  
		  ncutt++;
		}
	    }
      
      //printf("# cortes adicionados na restricao triangulo < 1: %d \n", ncutt);
      
      if(ncutt > 0)
	if(XPRSaddcuts(prob, ncutt, &mtype, &qrtype, &rhs, &mstart, &mrwind, &dmatval)) exit(9);
      
      ncut += ncutt;
      
    }// if(eq_trig == 1)
  
  return ncut;

}


int adicionar_bc_lin(XPRSprob prob, Array<int> &mtype, Array<char> &qrtype, Array<double> &rhs, Array <int> &mstart, Array<int> &mrwind, Array<double> &dmatval)
{
  int i, j, k;
  int nrow, nl;
  char var[64];
  Array<char> cnames;
  Array<Array<double> > y;
  int w, z, ncut = 0, ncutt;

  y.aloca(N);
  for(i=0;i<N;++i)
    y[i].redefine(N, 0);
  
  k = N;
  for(i=0;i<N-1;++i) 
    {
      for(j=0;j<N-i-1;++j, ++k)
	{
	  y[i][i+j+1] = x[k];
	}
    }
    
  int newrow = N*(N-1)/2, nz = N*(N-1), ad, l;
  qrtype.redefine(newrow, 'L');
  rhs.redefine(newrow, 1);
  mstart.aloca(newrow+1);
  mrwind.aloca(nz);
  dmatval.aloca(nz);
  ncutt = 0;
  mstart[0] = 0;
  for(i=0, z=0, w=0; i<N-1; ++i)
    for(j=i+1; j<N; ++j)
      {
	if( x[i] + x[j] - y[i][j] >= 1+0.0001)
	  {
	    //x_i
	    dmatval[w] = 1;
	    mrwind[w] = i;
	    
	    //x_j
	    dmatval[++w] = 1;
	    mrwind[w] = j;
	    
	    for(l=0, ad=N; l<i; ++l)
	      ad += N-l-1;
	    
	    //- y_ij
	    dmatval[++w] = -1;
	    mrwind[w] = ad + j -i -1;
	    
	    mstart[++z] = ++w;
	    
	    ncutt++;
	    
	  }
      }
  
  if(ncutt > 0)
    if(XPRSaddcuts(prob, ncutt, &mtype, &qrtype, &rhs, &mstart, &mrwind, &dmatval)) exit(9);
  
  ncut += ncutt;

  return ncut;
  
}
