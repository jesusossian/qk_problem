void fixaxy(XPRSprob prob, const double lp, const double best_sol, Array<int> &fix)
{
  int i,j;
  int nfix;
  double soma;
  
  fix.aloca(cx.getsize());

  nfix = 0;
  
  for(i=0, j=0; i<cx.getsize(); ++i)
    {
      //printf("x_%d %.2f cr_%d %.2f \n", i, x[i], i, cx[i]);
      if(cx[i] < 0.0001)
	{
	  soma = lp;
	  soma += cx[i];
	  if((soma < best_sol - 0.9999) && (stx[i]==0))
	    {
	      fix[j] = i;
	      stx[i] = 1;
	      ++j;
	      if(i<N) 
		{
		  qfx += 1;
		  //printf("fixou x %d \n", nfixax);
		}
	      if((N <= i) && (i < cx.getsize()))
		{
		  qfy += 1;
		  //printf("fixou y %d \n", nfixay);
		}     
	    }
	}
    }

  nfix = j;

  nfixtotal += nfix;

  //printf("qtd x fixas %d, qtd y fixas %d, total fixas %d\n", qfx, qfy, nfix);

  fix_xpress(prob, fix, nfix);

}


void fixay(XPRSprob prob, const double lp, const double best_sol, Array<int> &fix) {
  int i, j, k, l, nfix;
  double lp_sol;
  double soma;
  nfix = 0;
  soma = 0;
  
  fix.aloca(cx.getsize());

  for(i=0, k=N, l=0; i<N-1; ++i)
    {
      for(j=0; j<N-i-1; ++j, ++k)
	{
	  soma = lp;
	  if((cx[i] < 0.0001) || (cx[j+i+1] < 0.0001) || (cx[k] < 0.0001))
	    {
	      soma += cx[i];
	      soma += cx[j+i+1];
	      soma += cx[k];
	      if((soma <= best_sol - 0.9999) && (stx[k]==0))
		{
		  fix[l] = k;
		  stx[k] = 1;
		  ++l;
		}
	    }
	}
    }
 
  nfix = l;
  qfy += nfix;

  nfixtotal += nfix;

  //printf("qtd y fixas %d, total fixas %d\n", nfix, qfy + qfx);

  fix_xpress(prob, fix, nfix);

}

void fix_xpress(XPRSprob prob, const Array<int> &fix, int nfix)
{
  Array<int> mindex;
  Array<char> qbtype(nfix,'B');
  Array<double> bnd(nfix,0);

  mindex.aloca(nfix);
  for(int i=0; i<nfix; ++i)
    mindex[i] = fix[i];

  XPRSchgbounds(prob, nfix, &mindex, &qbtype, &bnd); 

}
