#include "utils.h"

static int total_cases = 0;
static int total_tests = 0;
static int passed_tests = 0;

void run_test_case(const char* case_name, void (*test_case)())
{
    printf("[ RUN      ] %s\n", case_name);
    test_case();
    printf("[       OK ] %s\n", case_name);
    total_cases++;
}

void expect_eq(int expected, int actual, const char* file, int line)
{
    total_tests++;
    if (expected != actual)
        printf("[  FAILED  ] Expected: %d, Actual: %d (%s:%d)\n", expected, actual, file, line);
    else
        passed_tests++;
}

void expect_str_eq(const char* expected, const char* actual, const char* file, int line)
{
    total_tests++;
    if (strcmp(expected, actual) != 0)
        printf("[  FAILED  ] Expected: %s, Actual: %s (%s:%d)\n", expected, actual, file, line);
    else
        passed_tests++;
}

void expect_arr_eq(const void* expected, const void* actual, size_t size, size_t elem_size, const char* file, int line)
{
    total_tests++;
    if (memcmp(expected, actual, size * elem_size) != 0)
        printf("[  FAILED  ] Arrays are not equal (%s:%d)\n", file, line);
    else
        passed_tests++;
}

void print_test_results()
{
    printf("[==========] %d tests from %d test cases ran.\n", total_tests, total_cases);
    printf("[  PASSED  ] %d tests.\n", passed_tests);
    if (total_tests - passed_tests > 0)
        printf("[  FAILED  ] %d tests.\n", total_tests - passed_tests);
}