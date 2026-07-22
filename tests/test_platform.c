/**
 * @file test_platform.c
 * @brief Unity unit tests for the platform module.
 *
 * Closes the "Partial" coverage gap noted for SRS-025 in
 * traceability/RTM.xlsx. platform_clear_screen() and
 * platform_wait_for_keypress() are inherently side-effecting /
 * blocking-on-input functions with little to assert on directly; they
 * are exercised here as smoke tests (must not crash) alongside more
 * thorough checks of platform_get_current_time(), which returns
 * assertable data.
 */

#include "unity.h"
#include "platform.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* platform_get_current_time() with a NULL pointer must not crash. */
void test_get_current_time_null_safety(void)
{
    platform_get_current_time(NULL);
    TEST_PASS();
}

/* platform_get_current_time() must fill plausible, in-range values. */
void test_get_current_time_returns_plausible_values(void)
{
    SystemTime t;
    memset(&t, -1, sizeof(t)); /* poison the struct so we can tell it was written */

    platform_get_current_time(&t);

    TEST_ASSERT_TRUE(t.day >= 1 && t.day <= 31);
    TEST_ASSERT_TRUE(t.month >= 1 && t.month <= 12);
    TEST_ASSERT_TRUE(t.year >= 2024); /* project began well after this */
    TEST_ASSERT_TRUE(t.hour >= 0 && t.hour <= 23);
    TEST_ASSERT_TRUE(t.minute >= 0 && t.minute <= 59);
}

/* platform_clear_screen() must not crash (smoke test; no console output
   assertion is made, since the effect is a terminal control sequence /
   system() call, not a return value). */
void test_clear_screen_does_not_crash(void)
{
    platform_clear_screen();
    TEST_PASS();
}

/* platform_wait_for_keypress() must not hang or crash when stdin is
   closed/exhausted (matches the same EOF-safety principle applied in
   ui_screens' ui_is_input_exhausted(), see main.c). This test relies on
   the test runner providing closed/empty stdin, which ctest does by
   default when no input is piped in. */
void test_wait_for_keypress_does_not_hang_on_closed_stdin(void)
{
    platform_wait_for_keypress();
    TEST_PASS();
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_current_time_null_safety);
    RUN_TEST(test_get_current_time_returns_plausible_values);
    RUN_TEST(test_clear_screen_does_not_crash);
    RUN_TEST(test_wait_for_keypress_does_not_hang_on_closed_stdin);

    return UNITY_END();
}
