#ifndef ACCEPTANCE_TEST_LIBRARY_H
#define ACCEPTANCE_TEST_LIBRARY_H

void lib_call_that_mallocs(char **buf);
void lib_take_an_out_param(char *buf);
void lib_do_nothing(void);
int lib_add_two_numbers(int a, int b);

#endif // ACCEPTANCE_TEST_LIBRARY_H
