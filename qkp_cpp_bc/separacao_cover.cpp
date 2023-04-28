double separacao_cover(pxpress * probl)
{
  int i;
  double objval_cover;
  Array<int> mindex(N);
  Array<double> obj(N);
  
  double sum_obj;
  double sol_cover;
  int bd;
  
  for(i=0;i<N;++i) 
    {
      mindex[i] = i;
      obj[i] = 1 - x[i];
    }
  
  if (XPRSchgobj(probl->cover, N, &mindex, &obj)) exit(9);
  
  if(pr_lp)
    XPRSwriteprob(probl->cover, "knapsack_cover", "l");
  
  XPRSmipoptimize(probl->cover, "");
  XPRSgetmipsol(probl->cover, &z, NULL);
  XPRSgetdblattrib(probl->cover,XPRS_MIPOBJVAL, &objval_cover);
  
  if(pr_lp == 1)
    {
      printf("solution separation : ");
      printf("z : ");
      for(i=0;i<N;++i)
	printf("%.2f ", z[i]);
      printf("\n");
    }
  
  //printf("objval_cover %.2f \n", objval_cover);
  
  return objval_cover;  

}
