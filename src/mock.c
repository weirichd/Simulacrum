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

void mock_off(mock_t *mock)
{
    mock->on = 0;
}

void mock_on(mock_t *mock)
{
    mock->on = 1;
}

int mock_is_enabled(mock_t *mock)
{
    return mock->on;
}
