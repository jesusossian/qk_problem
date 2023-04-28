void sort (lpitem *left, lpitem *right) {
  lpitem *r;

  if (right > left) {
    r = partesort (left, right);
    sort (left, r - 1);
    sort (r + 1, right);
  }
  
}

lpitem *partesort (lpitem *left, lpitem *right) {
  lpitem *i, *j;
  
  i = left;
  
  for (j = left+1; j <= right; ++j) {
    if (j->e > left->e) {
      ++i;
      SWAP(i, j);
    }
  }
  SWAP(left, i);
  
  return i;
  
}
