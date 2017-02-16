#include "mock.h"
#include <stdio.h>

void mock_set_callback(mock_t *mock, void *callback)
{
    mock->callback = callback;
}

void *mock_get_callback(mock_t *mock)
{
    return mock->callback;
}
