#include "cabecalho.h"

int main(int argc, char **argv)
{
  double *PI, **PIJ, *W, C;
  int i, j, t;
  //int i, j, k;
  FILE *fp;
  FILE *fpout;
  int pr_sol;
  double val;
  int initime;
  //Array<int> fix;
  time_t start, end;
  int elapsed;
  
  //int nfxa;
  //Array<double> bnd;
  //FILE* arq;
  //Array<int> mindex;
  
  fp = NULL;
  
  //pr_sol = 0;
  //pr_lp = 0;
  //solv_op = 2;
  //eq_ln = 0;
  //eq_trig0 = 0;
  //eq_trig1 = 0;
  //eq_cover = 0;
  //eq_extender = 0;
  //eq_lift = 0;
  //eq_cquad = 0;
  //eq_equad = 0;
  //eq_lquad = 0;
  //eq_mochquad = 0;
  //fixavx = 0;
  //fixavy = 0;
  //nfixtotal = 0;
  //nfix0 = 0;
  //cliquecr = 0;
  
  if (argc < 1 || argc > 10) {
    printf("Numero de parametros de entrada errado \n");
    return 0;
  }
  
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "cat: can't open %s\n", argv[1]);
    return 0;
  }
  
  //fscanf(fp,"%d", &N);
  //printf("%d\n",N);
  if (fscanf(fp, "%d", &(N)) != 1) {
    fprintf(stderr, "error : entrada 1 \n");
    exit(1);
  }
  
  //P.aloca(N);
  
  /*
    PIJ.aloca(N-1); 
  
  for (i=0; i<N-1; ++i)
    {
      if (N-i-1 <= 0)
	{
	  printf("Erro - tamanho do vetor invalido\n");
	  exit(0);
	}
      PIJ[i].aloca(N-i-1);
    }
  */
  
  //W.aloca(N);

  PI = calloc_vetor_double(N);
  PIJ = calloc_matriz_double(N,N);
  W = calloc_vetor_double(N);
  
  for (i=0;i<N;i++) {
    if (fscanf(fp, "%lf", &(PI[i])) != 1) {
      fprintf(stderr, "error : falta entrada \n");
      exit(1);
    }
    printf("%.1f ",PI[i]);
  }
  printf("\n");
  
  for (i=0;i<N;i++) {
    for (j=i+1;j<N;j++)	{
      if (fscanf(fp, "%lf", &(PIJ[i][j])) != 1) {
	fprintf(stderr, "error : falta entrada \n");
	exit(1);
      }
      printf("%.1f ", PIJ[i][j]);
    }
    printf("\n");
  }
  
  if (fscanf(fp, "%lf", &(C)) != 1) {
    fprintf(stderr, "error : entrada 1 \n");
    exit(1);
  }
  printf("%.1f \n", C);
  
  for (i=0;i<N;i++) {
    if (fscanf(fp, "%lf", &(W[i])) != 1) {
      fprintf(stderr, "error : falta entrada \n");
      exit(1);
    }
    printf("%.1f ", W[i]);
  }
  printf("\n");
  
  fclose(fp);
  
  fp = NULL;
  
  /*
  for (i=2; i<argc; ++i)
    {
      if (strcmp(argv[i], "-print_sol") == 0) pr_sol = 1; 
      else if (strcmp(argv[i], "-print_lp") == 0) pr_lp = 1; 
      else if (strcmp(argv[i], "-sol_lp") == 0) solv_op = 0; 
      else if (strcmp(argv[i], "-sol_lip") == 0) solv_op = 1; 
      else if (strcmp(argv[i], "-eq_ln_bc") == 0) eq_ln = 2; 
      else if (strcmp(argv[i], "-eq_ln_on") == 0) eq_ln = 1; 
      else if (strcmp(argv[i], "-eq_trig0_bc") == 0) eq_trig0 = 2; 
      else if (strcmp(argv[i], "-eq_trig0_on") == 0) eq_trig0 = 1;  
      else if (strcmp(argv[i], "-eq_trig1_bc") == 0) eq_trig1 = 2; 
      else if (strcmp(argv[i], "-eq_trig1_on") == 0) eq_trig1 = 1; 
      else if (strcmp(argv[i], "-eq_cover_bc") == 0) eq_cover = 1;
      else if (strcmp(argv[i], "-eq_cquad_bc") == 0) eq_cquad = 1;
      else if (strcmp(argv[i], "-eq_extender_bc") == 0) eq_extender = 1; 
      else if (strcmp(argv[i], "-eq_equad_bc") == 0) eq_equad = 1;
      else if (strcmp(argv[i], "-eq_lift_bc") == 0) eq_lift = 1; 
      else if (strcmp(argv[i], "-eq_lquad_bc") == 0) eq_lquad = 1;
      else if (strcmp(argv[i], "-eq_mochquad") == 0) eq_mochquad = 1;
      else if (strcmp(argv[i], "-proc_fixax") == 0) fixavx = 1;
      else if (strcmp(argv[i], "-proc_fixay") == 0) fixavy = 1;
      else if (strcmp(argv[i], "-proc_cliquecr") == 0) cliquecr = 1;
      else
	{
	  printf("Parametros de entrada errado \n");
	  return 0;
	}
    }
  */
  
  //XPRSprob prob;
  
  //if (XPRSinit(NULL)) exit(0);
  
  //if (XPRScreateprob(&prob)) exit(0);

  //XPRSsetlogfile(prob, "logprob.txt");  
  //modelo(prob);
  
  //stx.aloca(cx.getsize());
  //fix.aloca(cx.getsize());

  /*
  sty.aloca(N);
  for (i=0;i<N;++i)
    {
      sty[i].aloca(N);
    }
  
  cr.aloca(N);
  for (i=0;i<N;++i)
    {
      cr[i].aloca(N);
    }
  
  for (i=0; i<N-1; ++i)
    {
      for (j=i+1; j<N; ++j) {
	cr[i][j] = 0;
      }
    }
  
  for (i=0; i<cx.getsize(); ++i) stx[i]=0;
  */
  
  start = time(NULL);

  
  //p.aloca(N); 
  
  //for (i=0; i<N; ++i) p[i].aloca(N);
  
  //xprime.aloca(N);

  /*
  for (i=0; i<N; ++i) 
    p[i][i] = P[i];
  
  for (i=0; i<N-1; ++i)
    {
      for (j=0; j<N-i-1; ++j)
	{
	  p[i+j+1][i] = p[i][i+j+1] = PIJ[i][j];
	}
    }
  */
  
  //nclique = 0;
  //cutclique = 0;
  
  //greedy();
  
  //val = resolvecovercall(prob);

  end = time(NULL);
  elapsed = difftime(end, start);
  
  //printf("%.2f;%.2f;%.2f;%d;%.2f;%d;%d;%d;%d; \n", val, best_sol, boundr, cut0, bound0, qtdcut, qtdnodes, nit, elapsed);
  //fpout = fopen("saida.txt", "a");
  //fprintf(fpout, "%s;%.2f;%.2f;%.2f;%d;%.2f;%d;%d;%d;%d;%d;%d; \n", argv[1], val, best_sol, boundr, cut0, bound0, qtdcut, nfixax, nfixay, qtdnodes, nit, elapsed);
  //fprintf(fpout, "%s;%.2f;%.2f;%d;%d;%d;%d;%d; \n", argv[1], val, boundr, nfixax, nfixay, qtdcut, qtdnodes, elapsed);
  //fprintf(fpout, "%s;%.2f;%.2f;%d;%d;%d; \n", argv[1], val, boundr, qtdcut, qtdnodes, elapsed);
  //fclose (fpout);

  /*
  if (pr_sol)
    {
      printf("x = [");
      for (i=0;i<N;i++)
	printf( "%.2f ", xz[i]); 
      printf(" ] \n");
      printf("opt = %.2f \n",  val);
    }
  */
  
  //if(XPRSdestroyprob(prob)) exit(0);
  
  //if(XPRSfree()) exit(0);

  free_vetor_double(PI);
  free_matriz_double(N,PIJ);
  free_vetor_double(W);
  
  return 0;
  
}
