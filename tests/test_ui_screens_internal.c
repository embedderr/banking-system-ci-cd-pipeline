/**
 * @file test_ui_screens_internal.c
 * @brief Unity unit tests for ui_screens' internal input-parsing helpers.
 *
 * Closes the "Partial" coverage gap noted for SRS-027 in
 * traceability/RTM.xlsx (reject invalid numeric input rather than
 * proceeding with an indeterminate value). Uses ui_screens_internal.h,
 * a test-only header -- see that file's comment for why this exists.
 */

#include "unity.h"
#include "ui_screens_internal.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- parse_long --- */

void test_parse_long_valid(void)
{
    long out = 0;
    TEST_ASSERT_TRUE(parse_long("12345", &out));
    TEST_ASSERT_EQUAL_INT(12345, out);
}

void test_parse_long_negative(void)
{
    long out = 0;
    TEST_ASSERT_TRUE(parse_long("-42", &out));
    TEST_ASSERT_EQUAL_INT(-42, out);
}

void test_parse_long_rejects_trailing_garbage(void)
{
    /* This is the exact class of bug SRS-027 exists to prevent:
       "12abc" must be rejected entirely, not silently parsed as 12. */
    long out = 999;
    TEST_ASSERT_FALSE(parse_long("12abc", &out));
}

void test_parse_long_rejects_empty_string(void)
{
    long out = 999;
    TEST_ASSERT_FALSE(parse_long("", &out));
}

void test_parse_long_rejects_pure_alpha(void)
{
    long out = 999;
    TEST_ASSERT_FALSE(parse_long("abc", &out));
}

void test_parse_long_null_safety(void)
{
    long out = 999;
    TEST_ASSERT_FALSE(parse_long(NULL, &out));
}

/* --- parse_int --- */

void test_parse_int_valid(void)
{
    int out = 0;
    TEST_ASSERT_TRUE(parse_int("42", &out));
    TEST_ASSERT_EQUAL_INT(42, out);
}

void test_parse_int_rejects_trailing_garbage(void)
{
    int out = 999;
    TEST_ASSERT_FALSE(parse_int("4a", &out));
}

/* --- parse_long_long --- */

void test_parse_long_long_valid(void)
{
    long long out = 0;
    TEST_ASSERT_TRUE(parse_long_long("9876543210", &out));
    TEST_ASSERT_TRUE(out == 9876543210LL);
}

void test_parse_long_long_rejects_trailing_garbage(void)
{
    long long out = 999;
    TEST_ASSERT_FALSE(parse_long_long("123x", &out));
}

void test_parse_long_long_rejects_empty_string(void)
{
    long long out = 999;
    TEST_ASSERT_FALSE(parse_long_long("", &out));
}

/* --- parse_double --- */

void test_parse_double_valid(void)
{
    double out = 0.0;
    TEST_ASSERT_TRUE(parse_double("500.50", &out));
    TEST_ASSERT_EQUAL_DOUBLE(500.50, out);
}

void test_parse_double_rejects_trailing_garbage(void)
{
    double out = 999.0;
    TEST_ASSERT_FALSE(parse_double("500.50abc", &out));
}

void test_parse_double_rejects_empty_string(void)
{
    double out = 999.0;
    TEST_ASSERT_FALSE(parse_double("", &out));
}

/* --- copy_bounded --- */

void test_copy_bounded_fits(void)
{
    char dest[10];
    TEST_ASSERT_TRUE(copy_bounded(dest, sizeof(dest), "hello"));
    TEST_ASSERT_EQUAL_STRING("hello", dest);
}

void test_copy_bounded_rejects_oversized_input(void)
{
    /* dest can hold 5 chars + null; "hello world" is far longer and
       must be rejected outright, not silently truncated. */
    char dest[6];
    TEST_ASSERT_FALSE(copy_bounded(dest, sizeof(dest), "hello world"));
}

void test_copy_bounded_exact_fit_boundary(void)
{
    /* "hello" is 5 chars; dest of size 6 fits exactly (5 chars + null). */
    char dest[6];
    TEST_ASSERT_TRUE(copy_bounded(dest, sizeof(dest), "hello"));

    /* dest of size 5 does NOT fit "hello" (needs 6 for the null terminator). */
    char dest_too_small[5];
    TEST_ASSERT_FALSE(copy_bounded(dest_too_small, sizeof(dest_too_small), "hello"));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_parse_long_valid);
    RUN_TEST(test_parse_long_negative);
    RUN_TEST(test_parse_long_rejects_trailing_garbage);
    RUN_TEST(test_parse_long_rejects_empty_string);
    RUN_TEST(test_parse_long_rejects_pure_alpha);
    RUN_TEST(test_parse_long_null_safety);

    RUN_TEST(test_parse_int_valid);
    RUN_TEST(test_parse_int_rejects_trailing_garbage);

    RUN_TEST(test_parse_long_long_valid);
    RUN_TEST(test_parse_long_long_rejects_trailing_garbage);
    RUN_TEST(test_parse_long_long_rejects_empty_string);

    RUN_TEST(test_parse_double_valid);
    RUN_TEST(test_parse_double_rejects_trailing_garbage);
    RUN_TEST(test_parse_double_rejects_empty_string);

    RUN_TEST(test_copy_bounded_fits);
    RUN_TEST(test_copy_bounded_rejects_oversized_input);
    RUN_TEST(test_copy_bounded_exact_fit_boundary);

    return UNITY_END();
}
