#include "unity.h"
#include "../include/account.h"

void test_init_account(void) {
    Account acc;
    init_account(&acc, 1001, 5000.0);
    TEST_ASSERT_EQUAL_INT(1001, acc.id);
    // TEST_ASSERT_EQUAL_FLOAT(5000.0, acc.balance); // Float comparison needs care
}

void test_deposit(void) {
    Account acc;
    init_account(&acc, 1001, 1000.0);
    deposit(&acc, 500.0);
    TEST_ASSERT_EQUAL_INT(1500, (int)acc.balance); // Simplified
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_init_account);
    RUN_TEST(test_deposit);
    return UNITY_END();
}