#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "acceptance_test_library.h"

void lib_call_that_mallocs(char **buf)
{
    *buf = (char *)malloc(16 * sizeof(char));
}

void lib_take_an_out_param(char *buf){
    char *str = "hey there";
    memcpy(buf, str, 10);
}

void lib_do_nothing(void){
    printf("I do nothing");
}

int lib_add_two_numbers(int a, int b)
{
    return a + b;
}
