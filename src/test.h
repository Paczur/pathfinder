#ifndef H_TEST
#define H_TEST

#define TEST(function, case) static void function##_test_##case(void **state)
#define ADD(function, case) cmocka_unit_test(function##_test_##case)
#define SETUP(function) static int function##_test_setup(void **state)
#define TEARDOWN(function) static int function##_test_teardown(void **state)
#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))
#define assert_int_bigger(x, y) assert_not_in_range(x, 0, y);
#define assert_int_smaller(x, y) assert_in_range(x, 0, y);
#define assert_int_bigger_or_equal(x, y) assert_int_bigger(x + 1, y);
#define assert_int_smaller_or_equal(x, y) assert_int_smaller(x, y + 1);

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <cmocka.h>

#endif
