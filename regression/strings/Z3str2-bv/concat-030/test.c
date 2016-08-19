#include <assert.h>
#include "../../cprover-string-hack.h"

int main()
{
  __CPROVER_string m;
  __CPROVER_string n;
  __CPROVER_string x;
  __CPROVER_string y;

  if (__CPROVER_string_equal(__CPROVER_string_concat(x, y), __CPROVER_string_concat(m, n))
      && (__CPROVER_string_length(m) == 1)
      && (__CPROVER_string_length(n) == 0)) {
    assert(0);
  }
  return 0;
}