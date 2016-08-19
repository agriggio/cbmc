#include <assert.h>
#include "../../cprover-string-hack.h"

int main()
{
  __CPROVER_string I;
  __CPROVER_string J;
  __CPROVER_string X;
  __CPROVER_string Y;

  if (__CPROVER_string_equal(__CPROVER_string_concat(X, __CPROVER_string_literal("a")), __CPROVER_string_concat(I, __CPROVER_string_concat(__CPROVER_string_literal("c"), J)))
      && (__CPROVER_string_length(X) == 0)) {
    assert(0);
  }
  return 0;
}