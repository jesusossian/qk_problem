void ordena_d(int *k, int left, int right, int *a) 
{
  int r;
  if (right > left)
    {
      r = particao(k, left, right, a);
      ordena_d(k, left, r - 1, a);
      ordena_d(k, r + 1, right, a);
    }
}
