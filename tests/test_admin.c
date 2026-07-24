/**
 * @file test_admin.c
 * @brief Unity unit tests for the admin module.
 *
 * Implements test cases UT-012 through UT-021 as specified in
 * docs/testing/unit_test_cases.xlsx.
 */

#include "unity.h"
#include "admin.h"

void setUp(void) {}
void tearDown(void) {}

/* UT-012: correct credentials -> true. */
void test_UT_012_correct_credentials(void)
{
    TEST_ASSERT_TRUE(admin_check_credentials("admin123", "helloworld"));
}

/* UT-013: correct username, wrong password -> false. */
void test_UT_013_wrong_password(void)
{
    TEST_ASSERT_FALSE(admin_check_credentials("admin123", "wrongpass"));
}

/* UT-014: wrong username, correct password -> false. */
void test_UT_014_wrong_username(void)
{
    TEST_ASSERT_FALSE(admin_check_credentials("wronguser", "helloworld"));
}

/* UT-015: both wrong -> false. */
void test_UT_015_both_wrong(void)
{
    TEST_ASSERT_FALSE(admin_check_credentials("wronguser", "wrongpass"));
}

/* UT-016: empty strings -> false. */
void test_UT_016_empty_strings(void)
{
    TEST_ASSERT_FALSE(admin_check_credentials("", ""));
}

/* UT-017: NULL username -> false. */
void test_UT_017_null_username(void)
{
    TEST_ASSERT_FALSE(admin_check_credentials(NULL, "helloworld"));
}

/* UT-018: NULL password -> false. */
void test_UT_018_null_password(void)
{
    TEST_ASSERT_FALSE(admin_check_credentials("admin123", NULL));
}

/* UT-019: both NULL -> false. */
void test_UT_019_both_null(void)
{
    TEST_ASSERT_FALSE(admin_check_credentials(NULL, NULL));
}

/* UT-020: username differing only in case -> false (case-sensitive). */
void test_UT_020_username_case_sensitive(void)
{
    TEST_ASSERT_FALSE(admin_check_credentials("Admin123", "helloworld"));
}

/* UT-021: password differing only in case -> false (case-sensitive). */
void test_UT_021_password_case_sensitive(void)
{
    TEST_ASSERT_FALSE(admin_check_credentials("admin123", "HelloWorld"));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_UT_012_correct_credentials);
    RUN_TEST(test_UT_013_wrong_password);
    RUN_TEST(test_UT_014_wrong_username);
    RUN_TEST(test_UT_015_both_wrong);
    RUN_TEST(test_UT_016_empty_strings);
    RUN_TEST(test_UT_017_null_username);
    RUN_TEST(test_UT_018_null_password);
    RUN_TEST(test_UT_019_both_null);
    RUN_TEST(test_UT_020_username_case_sensitive);
    RUN_TEST(test_UT_021_password_case_sensitive);

    return UNITY_END();
}
