/**
 * @file test_cash_queue.c
 * @brief Unity unit tests for the cash_queue module.
 *
 * Implements test cases UT-040 through UT-052 as specified in
 * docs/testing/unit_test_cases.xlsx.
 */

#include "unity.h"
#include "cash_queue.h"

void setUp(void) {}
void tearDown(void) {}

/* UT-040: init -> is_empty true. */
void test_UT_040_init_then_empty(void)
{
    CashQueue q;
    cash_queue_init(&q);
    TEST_ASSERT_TRUE(cash_queue_is_empty(&q));
}

/* UT-041: dequeue on empty queue. */
void test_UT_041_dequeue_empty(void)
{
    CashQueue q;
    CashRequestInput out;
    cash_queue_init(&q);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_QUEUE_EMPTY, cash_queue_dequeue(&q, &out));
}

/* UT-042: peek_front on empty queue. */
void test_UT_042_peek_front_empty(void)
{
    CashQueue q;
    CashRequestInput out;
    cash_queue_init(&q);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_QUEUE_EMPTY, cash_queue_peek_front(&q, &out));
}

/* UT-043: enqueue one deposit and one withdrawal request. */
void test_UT_043_enqueue_deposit_and_withdraw(void)
{
    CashQueue q;
    CashRequestInput deposit  = { 1007750, 500.00, CASH_TXN_DEPOSIT };
    CashRequestInput withdraw = { 1007751, 200.00, CASH_TXN_WITHDRAW };
    cash_queue_init(&q);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, cash_queue_enqueue(&q, &deposit));
    TEST_ASSERT_EQUAL_INT(STATUS_OK, cash_queue_enqueue(&q, &withdraw));
    TEST_ASSERT_FALSE(cash_queue_is_empty(&q));
}

/* UT-044: peek_front returns correct type/account/amount without removing. */
void test_UT_044_peek_front_correct_fields(void)
{
    CashQueue q;
    CashRequestInput deposit = { 1007750, 500.00, CASH_TXN_DEPOSIT };
    CashRequestInput peeked;
    cash_queue_init(&q);

    cash_queue_enqueue(&q, &deposit);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, cash_queue_peek_front(&q, &peeked));
    TEST_ASSERT_EQUAL_INT(1007750, peeked.account_number);
    TEST_ASSERT_EQUAL_DOUBLE(500.00, peeked.amount);
    TEST_ASSERT_EQUAL_INT(CASH_TXN_DEPOSIT, peeked.type);
}

/* UT-045: FIFO order via iteration, mixed transaction types. */
void test_UT_045_fifo_order_via_iteration(void)
{
    CashQueue q;
    CashRequestInput r1 = { 1007750, 500.00, CASH_TXN_DEPOSIT };
    CashRequestInput r2 = { 1007751, 200.00, CASH_TXN_WITHDRAW };
    CashRequestInput r3 = { 1007752, 999.99, CASH_TXN_DEPOSIT };
    cash_queue_init(&q);

    cash_queue_enqueue(&q, &r1);
    cash_queue_enqueue(&q, &r2);
    cash_queue_enqueue(&q, &r3);

    const CashNode *node = cash_queue_get_front_node(&q);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(1007750, node->data.account_number);
    TEST_ASSERT_EQUAL_INT(CASH_TXN_DEPOSIT, node->data.type);

    node = cash_queue_get_next_node(node);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(1007751, node->data.account_number);
    TEST_ASSERT_EQUAL_INT(CASH_TXN_WITHDRAW, node->data.type);

    node = cash_queue_get_next_node(node);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(1007752, node->data.account_number);
    TEST_ASSERT_EQUAL_INT(CASH_TXN_DEPOSIT, node->data.type);

    node = cash_queue_get_next_node(node);
    TEST_ASSERT_NULL(node);
}

/* UT-046: FIFO order via dequeue, all fields round-trip correctly. */
void test_UT_046_fifo_order_via_dequeue(void)
{
    CashQueue q;
    CashRequestInput r1 = { 1007750, 500.00, CASH_TXN_DEPOSIT };
    CashRequestInput r2 = { 1007751, 200.00, CASH_TXN_WITHDRAW };
    CashRequestInput out;
    cash_queue_init(&q);

    cash_queue_enqueue(&q, &r1);
    cash_queue_enqueue(&q, &r2);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, cash_queue_dequeue(&q, &out));
    TEST_ASSERT_EQUAL_INT(1007750, out.account_number);
    TEST_ASSERT_EQUAL_DOUBLE(500.00, out.amount);
    TEST_ASSERT_EQUAL_INT(CASH_TXN_DEPOSIT, out.type);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, cash_queue_dequeue(&q, &out));
    TEST_ASSERT_EQUAL_INT(1007751, out.account_number);
    TEST_ASSERT_EQUAL_DOUBLE(200.00, out.amount);
    TEST_ASSERT_EQUAL_INT(CASH_TXN_WITHDRAW, out.type);
}

/* UT-047: dequeue after full drain. */
void test_UT_047_dequeue_after_full_drain(void)
{
    CashQueue q;
    CashRequestInput r1 = { 1007750, 500.00, CASH_TXN_DEPOSIT };
    CashRequestInput out;
    cash_queue_init(&q);

    cash_queue_enqueue(&q, &r1);
    cash_queue_dequeue(&q, &out);

    TEST_ASSERT_TRUE(cash_queue_is_empty(&q));
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_QUEUE_EMPTY, cash_queue_dequeue(&q, &out));
}

/* UT-048: Bug-regression -- re-enqueue after full drain correctly resets
   front/rear pointers. */
void test_UT_048_reenqueue_after_full_drain(void)
{
    CashQueue q;
    CashRequestInput r1 = { 1007750, 500.00, CASH_TXN_DEPOSIT };
    CashRequestInput r2 = { 1007751, 200.00, CASH_TXN_WITHDRAW };
    CashRequestInput out;
    cash_queue_init(&q);

    cash_queue_enqueue(&q, &r1);
    cash_queue_dequeue(&q, &out);
    TEST_ASSERT_TRUE(cash_queue_is_empty(&q));

    TEST_ASSERT_EQUAL_INT(STATUS_OK, cash_queue_enqueue(&q, &r2));
    TEST_ASSERT_FALSE(cash_queue_is_empty(&q));
    TEST_ASSERT_EQUAL_INT(STATUS_OK, cash_queue_dequeue(&q, &out));
    TEST_ASSERT_EQUAL_INT(1007751, out.account_number);
    TEST_ASSERT_TRUE(cash_queue_is_empty(&q));
}

/* UT-049: enqueue with NULL queue or NULL input. */
void test_UT_049_enqueue_null_safety(void)
{
    CashQueue q;
    CashRequestInput r1 = { 1007750, 500.00, CASH_TXN_DEPOSIT };
    cash_queue_init(&q);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, cash_queue_enqueue(NULL, &r1));
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, cash_queue_enqueue(&q, NULL));
}

/* UT-050: dequeue / peek_front with NULL queue. */
void test_UT_050_dequeue_and_peek_null_safety(void)
{
    CashRequestInput out;
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, cash_queue_dequeue(NULL, &out));
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, cash_queue_peek_front(NULL, &out));
}

/* UT-051: get_front_node / get_next_node NULL safety. */
void test_UT_051_iteration_null_safety(void)
{
    TEST_ASSERT_NULL(cash_queue_get_front_node(NULL));
    TEST_ASSERT_NULL(cash_queue_get_next_node(NULL));
}

/* UT-052: destroy with remaining nodes, and destroy with NULL. */
void test_UT_052_destroy(void)
{
    CashQueue q;
    CashRequestInput r1 = { 1007750, 500.00, CASH_TXN_DEPOSIT };
    CashRequestInput r2 = { 1007751, 200.00, CASH_TXN_WITHDRAW };
    cash_queue_init(&q);

    cash_queue_enqueue(&q, &r1);
    cash_queue_enqueue(&q, &r2);
    cash_queue_destroy(&q);
    TEST_ASSERT_TRUE(cash_queue_is_empty(&q));

    cash_queue_destroy(NULL);
    TEST_PASS();
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_UT_040_init_then_empty);
    RUN_TEST(test_UT_041_dequeue_empty);
    RUN_TEST(test_UT_042_peek_front_empty);
    RUN_TEST(test_UT_043_enqueue_deposit_and_withdraw);
    RUN_TEST(test_UT_044_peek_front_correct_fields);
    RUN_TEST(test_UT_045_fifo_order_via_iteration);
    RUN_TEST(test_UT_046_fifo_order_via_dequeue);
    RUN_TEST(test_UT_047_dequeue_after_full_drain);
    RUN_TEST(test_UT_048_reenqueue_after_full_drain);
    RUN_TEST(test_UT_049_enqueue_null_safety);
    RUN_TEST(test_UT_050_dequeue_and_peek_null_safety);
    RUN_TEST(test_UT_051_iteration_null_safety);
    RUN_TEST(test_UT_052_destroy);

    return UNITY_END();
}
