#include <check.h>
#include <stdlib.h>
#include <stdio.h>

#include "../src/mock.h"

#include "acceptance_test_library.h"

START_TEST(mock_t_sets_a_callback)
{
    mock_t actual = {0};
    char *callback;

    mock_set_callback(&actual, &callback);

    ck_assert_ptr_ne(actual.callback, NULL);
    ck_assert_ptr_eq(actual.callback, &callback);
}
END_TEST

START_TEST(mock_t_gets_a_callback)
{
    mock_t actual = {0};
    char *callback;

    mock_set_callback(&actual, &callback);

    ck_assert_ptr_ne(mock_get_callback(&actual), NULL);
    ck_assert_ptr_eq(mock_get_callback(&actual), &callback);
}
END_TEST

void test_callback(char *buf)
{
    char *str = "hello";
    memcpy(buf, str, 6);
}

typedef void (*mock_test_callback)(char *buf);

START_TEST(mock_t_calls_a_callback)
{
    mock_t mock = {0};
    char *actual = calloc(6, sizeof(char));

    mock_set_callback(&mock, &test_callback);

    ((mock_test_callback)mock_get_callback(&mock))(actual);

    ck_assert_str_eq(actual, "hello");
}
END_TEST

START_TEST(mock_t_dispels_a_mock)
{
    mock_t mock = {.on = 1};

    mock_off(&mock);

    ck_assert_int_eq(mock.on, 0);
}
END_TEST

START_TEST(mock_t_enables_a_mock)
{
    mock_t mock = {.on = 0};

    mock_on(&mock);

    ck_assert_int_eq(mock.on, 1);
}
END_TEST

Suite *make_mock_unit_test_suite()
{
    Suite *s;
    TCase *tc;

    s = suite_create("Mock unit tests");
    tc = tcase_create("Core");

    tcase_add_test(tc, mock_t_sets_a_callback);
    tcase_add_test(tc, mock_t_gets_a_callback);
    tcase_add_test(tc, mock_t_calls_a_callback);
    tcase_add_test(tc, mock_t_dispels_a_mock);
    tcase_add_test(tc, mock_t_enables_a_mock);

    suite_add_tcase(s, tc);

    return s;
}

// Acceptance tests

SIMULACRUM(void, malloc, 0)

START_TEST(it_should_noop_malloc)
{
    char *actual = {0};

    lib_call_that_mallocs(&actual);

    ck_assert_ptr_eq(actual, NULL);
}
END_TEST

START_TEST(it_should_disable_noop_and_call_malloc)
{
    char *actual = {0};

    mock_off(&malloc_mock);

    lib_call_that_mallocs(&actual);

    ck_assert_ptr_ne(actual, NULL);

    free(actual);
}
END_TEST

SIMULACRUM(void, free, 0)

START_TEST(it_should_noop_free)
{
    char *actual = "hello world";

    free(actual);
}
END_TEST

SIMULACRUM(void, lib_take_an_out_param, 1, char *)

START_TEST(it_should_execute_a_callback)
{
   char *actual = calloc(6, sizeof(char));

   mock_set_callback(&lib_take_an_out_param_mock, &test_callback);

   lib_take_an_out_param(actual);

   ck_assert_ptr_eq(lib_take_an_out_param_mock.callback, &test_callback);
   ck_assert_str_eq(actual, "hello");
}
END_TEST

// TODO: Split into separate module, add run flag
Suite *acceptance_tests()
{
    Suite *s;
    TCase *tc;

    s = suite_create("Mock acceptance tests");
    tc = tcase_create("Core");

    tcase_add_test(tc, it_should_noop_malloc);
    tcase_add_test(tc, it_should_disable_noop_and_call_malloc);
    tcase_add_test(tc, it_should_noop_free);
    tcase_add_test(tc, it_should_execute_a_callback);

    suite_add_tcase(s, tc);

    return s;
}

int main()
{
    int number_failed;
    SRunner *sr;

    sr = srunner_create(make_mock_unit_test_suite());
    srunner_add_suite(sr, acceptance_tests());

    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed==0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
