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

START_TEST(mock_t_increments_and_gets_call_count)
{
    mock_t mock = {.call_count = 0};

    mock_increment_call_count(&mock);

    ck_assert_int_eq(mock_get_call_count(&mock), 1);
}
END_TEST

START_TEST(mock_t_sets_and_gets_a_str_return_value)
{
  mock_t mock = {0};
  char *expected = "hello world";

  mock_set_return_value(&mock, &expected);

  ck_assert_str_eq(*((char **)mock_get_return_value(&mock)), expected);
}
END_TEST

START_TEST(mock_t_sets_and_gets_an_int_return_value)
{
  mock_t mock = {0};
  int expected = 10;

  mock_set_return_value(&mock, &expected);

  ck_assert_int_eq(*((int *)mock_get_return_value(&mock)), expected);
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
    tcase_add_test(tc, mock_t_increments_and_gets_call_count);
    tcase_add_test(tc, mock_t_sets_and_gets_a_str_return_value);
    tcase_add_test(tc, mock_t_sets_and_gets_an_int_return_value);

    suite_add_tcase(s, tc);

    return s;
}

// Acceptance tests

SIMULACRUM(void, malloc, 0)

START_TEST(it_should_noop_malloc)
{
    char *actual = NULL;

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

SIMULACRUM(void, lib_do_nothing, 0)

START_TEST(it_should_maintain_a_call_count)
{
    lib_do_nothing();
    lib_do_nothing();
    lib_do_nothing();
    lib_do_nothing();

    ck_assert_int_eq(mock_get_call_count(&lib_do_nothing_mock), 4);
}
END_TEST

SIMULACRUM(int, lib_add_two_numbers, 2, int, int)

START_TEST(it_should_allow_custom_return_value)
{
    int actual;
    int expected = 10;

    mock_set_return_value(&lib_add_two_numbers_mock, &expected);

    actual = lib_add_two_numbers(1, 2);

    ck_assert_int_eq(actual, 10);
}
END_TEST

void fill_out_param_callback(char *buf, int size, void *file)
{
    char *mock_file_line = "A line in a fake file";
    memcpy(buf, mock_file_line, size);
}

SIMULACRUM(char *, fgets, 3, char *, int, void *)

START_TEST(it_uses_a_callback_to_set_out_param)
{
    int size = 32;
    char buf[48];
    char *rtn = "hello world";
    char *actual;

    mock_set_callback(&fgets_mock, &fill_out_param_callback);
    mock_set_return_value(&fgets_mock, &rtn);

    actual = fgets(buf, size, NULL);

    ck_assert_str_eq(actual, "hello world");
    ck_assert_str_eq(buf, "A line in a fake file");
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
    tcase_add_test(tc, it_should_maintain_a_call_count);
    tcase_add_test(tc, it_should_allow_custom_return_value);
    tcase_add_test(tc, it_uses_a_callback_to_set_out_param);

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
