#include <check.h>
#include <stdlib.h>
#include "acceptance_test_library.h"

// TODO: remove this and implement 
#define MOCK(...)

START_TEST(assert_true)
{
    ck_assert(1);
}
END_TEST

// Acceptance tests

MOCK(void, malloc, size_t);

START_TEST(it_should_noop_malloc)
{
    char *actual = {0};

    lib_call_that_mallocs(&actual);

    ck_assert_ptr_eq(actual, NULL);
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

    suite_add_tcase(s, tc);

    return s;
}

Suite *make_mock_unit_test_suite()
{
    Suite *s; 
    TCase *tc;

    s = suite_create("Mock unit tests");
    tc = tcase_create("Core");

    tcase_add_test(tc, assert_true);

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
