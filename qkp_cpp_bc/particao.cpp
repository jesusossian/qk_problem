int particao(int *k, int left, int right, int *a)
{
  int i, j, tp;
  i = left;

  for (j = left+1; j <= right; ++j)
    {
      if (k[j] > k[left]) {
	++i;
	troca(&a[i], &a[j]);
      }
    }
  troca(&a[left], &a[i]);
  
  return i;

}
