void greedy(void)
{
  int i, j, mini;
  double eff, mineff;
  int psum, wsum, pi, minp;
  int n;
  
  n = N;
  
  psum = wsum = 0;
  
  for (i = 0; i < n; i++)
    {
      xp[i] = 0;
      wsum += W[i];
      for (j = 0; j < n; j++) 
	{
	  psum += p[i][j];
	}
    }
  ptot = psum;
  wtot = wsum;
  
  for (i = 0; i < n; i++)
    {
      xp[i] = 1;
    }
  psum = ptot;
  wsum = wtot;
  
  for (;;) 
    {
      mineff = ptot;
      mini = -1;
      for (i = 0; i < n; i++)
	{
	  if (xp[i] != 1) continue;
	  pi = -p[i][i];
	  for (j = 0; j < n; j++) 
	    {
	      if (xp[j])
		pi += p[i][j] + p[j][i];
	    }
	  eff = pi / (double)W[i];
	  
	  if (eff < mineff) 
	    {
	      mineff = eff;
	      mini = i;
	      minp = pi;
	    }
	}
      
      if (mini == -1)
	{
	  printf("item nao encontrado \n");
	  exit(-1);
	}
      i = mini;
      xp[i] = 0;
      psum -= minp;
      wsum -= W[i];
      if (wsum <= C) break;
    }
  
  best_sol = psum;
  
  improve(xp);
  
}

void improve(Array<int> &xprime)
{
  int i, j, gaini, gainj;
  int tot, gain, bgain, res;
  int n = N;
  int q[n];
  
  res = C;
  /*printf("improve n %d res %d c %d \n", n, res, cap);*/
  
  for (i = 0; i < n; i++)
    {
      if (xprime[i]) res -= W[i];
    }
  
  for ( ; ; ) 
    {
      for (i = 0; i < n; i++) 
	{
	  for (tot = p[i][i], j = 0; j < n; j++) 
	    {
	      if ((j != i) && (xprime[j] != 0)) 
		{
		  tot += p[i][j] + p[j][i];
		}
	    }
	  q[i] = tot;
	}
      
      bgain = gaini = gainj = 0;
      for (i = 0; i < n; i++) 
	{
	  if (xprime[i] == 0) 
	    {
	      if (W[i] <= res) 
		{
		  gain = q[i];
		  if (gain > bgain)
		    {
		      bgain = gain;
		      gaini = i;
		      gainj = -1;
		    }
		} 
	      else 
		{
		  for (j = 0; j < n; j++) 
		    {
		      if (j == i) continue;
		      if (xprime[j] == 0) continue;
		      if (W[i] - W[j] <= res) 
			{
			  gain = q[i] - q[j] - (p[i][j] + p[j][i]);
			  if (gain > bgain) 
			    {
			      bgain = gain;
			      gaini = i;
			      gainj = j;
			    }
                        }
		    }
		}
	    }
	}
      /*printf("best gain %d i %d j %d res %d\n", bgain, gaini, gainj, res);*/
      if (bgain == 0) break;
      
      xprime[gaini] = 1;
      
      if (gainj != -1) xprime[gainj] = 0;
      
      if (gainj != -1) 
	{
	  res += W[gainj] - W[gaini];
	} 
      else 
	{
	  res -= W[gaini];
	}
      
      if (res < 0)
	{
	  printf("\n mochila cheia");
	  exit(-1);
	}
    }
  
  for (gain = 0, res = C, i = 0; i < n; i++) 
    {
      if (xprime[i] == 0) continue;
      res -= W[i];
      
      for (j = 0; j < n; j++) 
	{
	  if (xprime[j]) 
	    {
	      gain += p[i][j];
	    }
	}
    }

  if (res < 0)
    {
      printf("\n excesso de capacidade ");
      exit(-1);
    }
  /*printf("possible solution %d\n", gain);*/
  
  if (gain > best_sol) 
    {
      best_sol = gain;
    }

}
