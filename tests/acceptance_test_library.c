#include "acceptance_test_library.h"
#include <stdlib.h>

void lib_call_that_mallocs(char **buf)
{
    *buf = (char *)malloc(16 * sizeof(char));
}

