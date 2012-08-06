#include <stdlib.h>
#include <string.h>

#include "fstr.h"

int fstrcat(fstr * d, const fstr * s1, const fstr * s2) {
  d->len = s1->len + s2->len;
  d->c_str = malloc(d->len + 1);
  d->flags |= FSTR_TEMPORARY;

  if(d->c_str == NULL)
    return 1;
  
  memcpy(d->c_str,         s1->c_str, s1->len);
  memcpy(d->c_str+s1->len, s2->c_str, s2->len);
  d->c_str[d->len] = '\0';

  return 0;
}

void fstrfree(fstr * d) {
  if(d == NULL || d->c_str == NULL || (~d->flags) & FSTR_TEMPORARY)
    return;
  
  free(d->c_str);
  return;
}
