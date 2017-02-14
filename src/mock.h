#ifndef MOCK_H
#define MOCK_H

#include <stdio.h>

#define TEST_DOUBLE(__function_name)    \
void *__wrap_##__function_name(void)    \
{                                       \
  return NULL;                          \
}                                       \

#endif //MOCK_H
