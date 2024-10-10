#pragma once

/**
 * @file utils.h
 * @author Xiaojintao
 * @brief 
 * @version 0.1
 * @date 2024-08-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_CASE(case_name) void case_name()
#define RUN_TEST_CASE(case_name) run_test_case(#case_name, case_name)
#define EXPECT_EQ(expected, actual) expect_eq((expected), (actual), __FILE__, __LINE__)
#define EXPECT_STR_EQ(expected, actual) expect_str_eq((expected), (actual), __FILE__, __LINE__)
#define EXPECT_ARR_EQ(expected, actual, size) expect_arr_eq((expected), (actual), (size), sizeof(*(expected)), __FILE__, __LINE__)

void run_test_case(const char* case_name, void (*test_case)());
void expect_eq(int expected, int actual, const char* file, int line);
void expect_str_eq(const char* expected, const char* actual, const char* file, int line);
void expect_arr_eq(const void* expected, const void* actual, size_t size, size_t elem_size, const char* file, int line);
void print_test_results();

#ifdef __cplusplus
}
#endif