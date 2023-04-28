double *bnd;
char *qbtype;
int nbnds;

int XPRS_CC cut_manager(XPRSprob prob, void *uobj)
{
  double objval, objval_cover;
  int i, j, k, starttime,  ncut;
  Array<int> mindex(N), mtype(1,nit), mstart(2), mcols(N);
  Array<double> obj(N), drhs(1), dmatval(N);
  int node, card_ext, max;
  XPRSprob prob_cover;
  Array<char> qrtype(1,'L');
  Array<int> mrwind;
  Array<int> fix;
  int nCols, nRows;
  double rcval;
  int fixanew;

  pxpress * probl; 
  
  starttime = (unsigned long int) clock();
  
  probl = (pxpress *) uobj;
  
  ++nit;
  
  XPRSgetdblattrib(prob,XPRS_LPOBJVAL,&objval);
  XPRSgetintattrib(prob, XPRS_NODES, &node);
  XPRSgetintattrib(prob,XPRS_MIPSTATUS,&xpress_status);

  if (node > nodeold) itnode = 0;
  else itnode++;

  //printf("iteracao: %d, fobj: %f\n", nit, objval);

  if (nit == 1)
    {
      boundr = objval;
    }
  
  ncut = 0;
  fixanew = 1;
  
  if ( (xpress_status==XPRS_MIP_OPTIMAL) || (xpress_status==XPRS_MIP_SOLUTION) )
   {
     printf("encontrou solução otima \n");
     if (XPRSgetlpsol(prob, &xz, NULL, NULL, NULL)) exit(9);
   }

   if (XPRSgetlpsol(prob, &x, NULL, NULL, &cx)) exit(9);

  if (pr_lp == 1)
    {
      for (i=0;i<N;++i)
	{
	  printf("x_%d : %.2f  ", i+1, x[i]);
	}
      printf("\n");
      
      k = N;
      for (i=0;i<N-1;++i)
	{
	  for (j=0;j<N-i-1;++j,++k)
	    {
	      printf("y_%d_%d : %.2f ", i+1, i+j+2, x[k]);
	    }
	  printf("\n");
	}
      printf("\n");
    }
  
  //if(XPRSdelcuts(prob, 0, NULL, -1, XPRS_MINUSINFINITY, -1, NULL)) exit(9);
  //if(XPRSdelcpcuts(prob, NULL, -1, -1, NULL)) exit(9);

  if (eq_ln == 2)
    {
      if (itnode < MAX_ITER_SEP) 
	ncut += adicionar_bc_lin(prob, mtype, qrtype, drhs, mstart, mrwind, dmatval);
    }
  
  if ((eq_trig0 == 2) || (eq_trig1 == 2))
    {
      if (itnode < MAX_ITER_SEP) 
	ncut += adicionar_bc_tri(prob, mtype, qrtype, drhs, mstart, mrwind, dmatval);
    }
  
  if ((eq_cover == 1) || (eq_extender == 1) || (eq_lift == 1) || (eq_cquad == 1) || (eq_equad == 1) || (eq_lquad == 1))
    {
      objval_cover = separacao_cover(probl);
      
      if (objval_cover <= 1-0.0001)
	{

	  if (eq_cover == 1)
	    {
	      if (itnode < MAX_ITER_SEP) 
		ncut += adicionar_bc_cover(prob, mtype, qrtype, drhs, mstart, objval_cover, mcols, dmatval);
	    }
	  
	  if (eq_extender == 1)
	    {
	      if (itnode < MAX_ITER_SEP)
		ncut += adicionar_bc_extender(prob, mtype, qrtype, drhs, mstart, objval_cover, mcols, dmatval);//extended
	    }
	  
	  if (eq_lift == 1)
	    {
	      if (itnode < MAX_ITER_SEP)
		ncut += adicionar_bc_lift(prob, mtype, qrtype, drhs, mstart, objval_cover, mcols, dmatval, probl);
	    }
	  
	  if (eq_cquad == 1)
	    {
	      if (itnode < MAX_ITER_SEP)
		ncut += adicionar_bc_cover_quad(prob, mtype, qrtype, drhs, mstart, objval_cover, mcols, dmatval);
	    }
	  
	  if (eq_equad == 1)
	    {
	      if (itnode < MAX_ITER_SEP)
		ncut += adicionar_bc_extender_quad(prob, mtype, qrtype, drhs, mstart, objval_cover, mcols, dmatval);
	    }
	  
	  if (eq_lquad == 1)
	    {
	      if (itnode < MAX_ITER_SEP)
		ncut += adicionar_bc_lift_quad(prob, mtype, qrtype, drhs, mstart, objval_cover, mcols, dmatval, probl);
	    }
	}
      
      if (pr_lp == 1)
	{
	  XPRSwriteprob(prob, "qknapsack_branch_cut", "l");
	}
  
    }

  // printf("onode %d node %d itnode %d -> ", nodeold, node, itnode);

  nodeold = node;

  ncutotal += ncut;
  
  XPRSgetintattrib(prob, XPRS_COLS, &nCols);
  XPRSgetintattrib(prob, XPRS_ROWS, &nRows);

  //fixa em todo nodes
  /*
  nbnds = 0;
  for(int i = 0; i < N; i++)
    {
      if (cx[i] < 0) rcval = -cx[i];
      else rcval = cx[i];
      if (rcval > objval - best_sol)
	{
	  mindex[nbnds] = i;
	  qbtype[nbnds] = 'B';
	  bnd[nbnds] = x[i];
	  //printf("rcval %.2f heuVal %.2f val %.2f \n", rcval, heuVal, val);
	  cout << "fixa " << i << " em " << x[i] << endl;
	  nbnds++;
	}
    }
  */

  qfx = 0;
  qfy = 0;
  if (node == 0)
    {
      cut0 = ncutotal;
      if (ncutotal == 0) bound0 = boundr;
      if (ncut == 0) bound0 = objval;
      /*
      if (fixanew == 1)
	{
	  cout << endl;
	  nbnds = 0;
	  for(int i = 0; i < N; i++)
	    {
	      if (cx[i] < 0) rcval = -cx[i];
	      else rcval = cx[i];
	      if (rcval > objval - best_sol)
		{
		  mindex[nbnds] = i;
		  qbtype[nbnds] = 'B';
		  bnd[nbnds] = x[i];
		  //printf("rcval %.2f heuVal %.2f val %.2f \n", rcval, heuVal, val);
		  cout << "fixa " << i << " em " << x[i] << endl;
		  nbnds++;
		}
	    }
	  cout << "#fixa = " << nbnds << endl;
	  
	  XPRSchgbounds(prob, nbnds, &mindex, qbtype, bnd);

	}
      */
      if (fixavx == 1)
	{
	  qfx = 0;
	  qfy = 0;
	  fixaxy(prob, objval, best_sol, fix);
	  nfixax += qfx;
	  nfixay += qfy;
	}

    }
  
  tempo = ( (double) ((unsigned long int) clock()-starttime) / CLOCKS_PER_SEC );
  
  //printf("It %d, node %d, (%.2f sec), opt %.2f, separacao %.2f, cuts : %d (tot %d), fx: %d fy: %d (tot %d) \n", 
  //nit, node, tempo, objval, objval_cover, ncut, ncutotal, nfixax, nfixay, (nfixtotal)/*, nCols, nRows*/);

  qtdnodes = node;  
  qtdcut = ncutotal;
  
  return ncut;

}

double resolvecovercall(XPRSprob prob)
{
  pxpress probl;
  int i;
  double val;

  bnd = (double *) malloc (N * sizeof(double));
  qbtype = (char *) malloc (N * sizeof(char));
  
  if (XPRSsetintcontrol(prob, XPRS_CUTSTRATEGY, 0)) exit(2);
  if (XPRSsetintcontrol(prob, XPRS_PRESOLVE, 0)) exit(2);
  if (XPRSsetintcontrol(prob, XPRS_HEURSTRATEGY, 0)) exit(2);
  if (XPRSsetintcontrol(prob, XPRS_LPLOG, 0)) exit(2);
  if (XPRSsetintcontrol(prob, XPRS_MIPLOG, 3)) exit(2);
  if (XPRSsetintcontrol(prob, XPRS_MIPTHREADS, 1)) exit(2);
  if (XPRSsetintcontrol(prob, XPRS_THREADS, 1)) exit(2);
  if (XPRSsetintcontrol(prob, XPRS_DEFAULTALG, 2)) exit(2);
  //if (XPRSsetintcontrol(prob, XPRS_BACKTRACK, 12)) exit(2);
  //if (XPRSsetintcontrol(prob, XPRS_NODESELECTION, 3)) exit(2);
  
  ncutotal = 0;
  nit = 0;
  nbnds = 0;

  XPRScreateprob(&probl.cover);
  XPRSsetlogfile(probl.cover, "logcover.txt");  
  modelo_cover(probl.cover);
  XPRSsetintcontrol(probl.cover, XPRS_OUTPUTLOG, 0);
  XPRSsetcbcutmgr(prob, cut_manager, &probl);
  XPRSchgobjsense(prob, XPRS_OBJ_MAXIMIZE);
  
  switch(solv_op)
    {
    case 0:
      XPRSsetintcontrol(prob, XPRS_MAXTIME, MAX_CPU_TIME);
      XPRSsetintcontrol(prob,XPRS_EXTRAROWS,MAX_NUM_CORTES+1);
      XPRSlpoptimize(prob, "d");
      XPRSgetlpsol(prob, &x, NULL, NULL, NULL);
      XPRSgetdblattrib(prob,XPRS_LPOBJVAL,&val);
      break;
    case 1:
      XPRSsetintcontrol(prob, XPRS_MAXTIME, MAX_CPU_TIME);
      XPRSsetintcontrol(prob,XPRS_EXTRAROWS,MAX_NUM_CORTES+1);
      XPRSmipoptimize(prob,"ld");
      XPRSgetlpsol(prob, &x, NULL, NULL, NULL);
      XPRSgetdblattrib(prob,XPRS_LPOBJVAL,&val);
      break;
    case 2:
      XPRSsetintcontrol(prob, XPRS_MAXTIME, MAX_CPU_TIME);
      XPRSsetintcontrol(prob,XPRS_EXTRAROWS,MAX_NUM_CORTES+1);
      XPRSmipoptimize(prob,"d");
      XPRSgetmipsol(prob, &x, NULL);
      XPRSgetdblattrib(prob,XPRS_MIPOBJVAL,&val);
      break;
    default:
      printf("Erro - resolve \n");
      exit(99);
    }
  
  //printf("resolvecovercall - solucao: %f \n", val);

  free(bnd); bnd = NULL;
  free(qbtype); qbtype = NULL;

  return val;

}

char * itoa(int n)
{
  register int r, k;
  int next;
  static char qbuf[8];
  int flag = 0;
  
  next = 0;
  if (n < 0)
    {
      qbuf[next++] = '-';
      n = -n;
    }
  
  if (n == 0)
    {
      qbuf[next++] = '0';
    }//if
  else
    {
      k = 10000;
      while (k > 0)
	{
	  r = n/k;
	  if (flag || r > 0)
	    {
	      qbuf[next++] = '0' + r;
	      flag = 1;
	    }//if
	  n -= r * k;
	  k  = k/10;
	}//while
    }//else
  
  qbuf[next] = 0;
  return(qbuf);  

}

void modelo (XPRSprob opt_prob) {

}
