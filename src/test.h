#ifndef H_TEST
#define H_TEST

#define TEST(function, case) static void function##_test_##case(void **state)
#define ADD(function, case) cmocka_unit_test(function##_test_##case)
#define SETUP(function) static int function##_test_setup(void **state)
#define TEARDOWN(function) static int function##_test_teardown(void **state)
#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <cmocka.h>

#endif
