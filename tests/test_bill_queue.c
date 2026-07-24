/**
 * @file test_bill_queue.c
 * @brief Unity unit tests for the bill_queue module.
 *
 * Implements test cases UT-022 through UT-039 as specified in
 * docs/testing/unit_test_cases.xlsx.
 */

#include "unity.h"
#include "bill_queue.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* UT-022: init -> is_empty true. */
void test_UT_022_init_then_empty(void)
{
    BillQueue q;
    bill_queue_init(&q);
    TEST_ASSERT_TRUE(bill_queue_is_empty(&q));
}

/* UT-023: dequeue on empty queue. */
void test_UT_023_dequeue_empty(void)
{
    BillQueue q;
    BillRequestInput out;
    bill_queue_init(&q);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_QUEUE_EMPTY, bill_queue_dequeue(&q, &out));
}

/* UT-024: peek_front on empty queue. */
void test_UT_024_peek_front_empty(void)
{
    BillQueue q;
    BillRequestInput out;
    bill_queue_init(&q);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_QUEUE_EMPTY, bill_queue_peek_front(&q, &out));
}

/* UT-025: enqueue single valid request. */
void test_UT_025_enqueue_single(void)
{
    BillQueue q;
    BillRequestInput req = { "Electricity", 1001, 500.00 };
    bill_queue_init(&q);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, bill_queue_enqueue(&q, &req));
    TEST_ASSERT_FALSE(bill_queue_is_empty(&q));
}

/* UT-026: peek_front does not remove; repeated peek returns same item. */
void test_UT_026_peek_front_does_not_remove(void)
{
    BillQueue q;
    BillRequestInput r1 = { "Electricity", 1001, 500.00 };
    BillRequestInput r2 = { "Gas",         1002, 300.00 };
    BillRequestInput r3 = { "Internet",    1003, 999.00 };
    BillRequestInput peeked;
    bill_queue_init(&q);

    bill_queue_enqueue(&q, &r1);
    bill_queue_enqueue(&q, &r2);
    bill_queue_enqueue(&q, &r3);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, bill_queue_peek_front(&q, &peeked));
    TEST_ASSERT_EQUAL_STRING("Electricity", peeked.bill_type);

    /* Peek again -- must still be the same front item. */
    TEST_ASSERT_EQUAL_INT(STATUS_OK, bill_queue_peek_front(&q, &peeked));
    TEST_ASSERT_EQUAL_STRING("Electricity", peeked.bill_type);
}

/* UT-027: FIFO order via iteration. */
void test_UT_027_fifo_order_via_iteration(void)
{
    BillQueue q;
    BillRequestInput r1 = { "Electricity", 1001, 500.00 };
    BillRequestInput r2 = { "Gas",         1002, 300.00 };
    BillRequestInput r3 = { "Internet",    1003, 999.00 };
    bill_queue_init(&q);

    bill_queue_enqueue(&q, &r1);
    bill_queue_enqueue(&q, &r2);
    bill_queue_enqueue(&q, &r3);

    const BillNode *node = bill_queue_get_front_node(&q);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_STRING("Electricity", node->data.bill_type);

    node = bill_queue_get_next_node(node);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_STRING("Gas", node->data.bill_type);

    node = bill_queue_get_next_node(node);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_STRING("Internet", node->data.bill_type);

    node = bill_queue_get_next_node(node);
    TEST_ASSERT_NULL(node);
}

/* UT-028: FIFO order via dequeue. */
void test_UT_028_fifo_order_via_dequeue(void)
{
    BillQueue q;
    BillRequestInput r1 = { "Electricity", 1001, 500.00 };
    BillRequestInput r2 = { "Gas",         1002, 300.00 };
    BillRequestInput r3 = { "Internet",    1003, 999.00 };
    BillRequestInput out;
    bill_queue_init(&q);

    bill_queue_enqueue(&q, &r1);
    bill_queue_enqueue(&q, &r2);
    bill_queue_enqueue(&q, &r3);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, bill_queue_dequeue(&q, &out));
    TEST_ASSERT_EQUAL_STRING("Electricity", out.bill_type);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, bill_queue_dequeue(&q, &out));
    TEST_ASSERT_EQUAL_STRING("Gas", out.bill_type);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, bill_queue_dequeue(&q, &out));
    TEST_ASSERT_EQUAL_STRING("Internet", out.bill_type);
}

/* UT-029: dequeue after full drain. */
void test_UT_029_dequeue_after_full_drain(void)
{
    BillQueue q;
    BillRequestInput r1 = { "Electricity", 1001, 500.00 };
    BillRequestInput out;
    bill_queue_init(&q);

    bill_queue_enqueue(&q, &r1);
    bill_queue_dequeue(&q, &out);

    TEST_ASSERT_TRUE(bill_queue_is_empty(&q));
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_QUEUE_EMPTY, bill_queue_dequeue(&q, &out));
}

/* UT-030: Bug-regression -- re-enqueue after full drain correctly resets
   front/rear pointers. */
void test_UT_030_reenqueue_after_full_drain(void)
{
    BillQueue q;
    BillRequestInput r1 = { "Electricity", 1001, 500.00 };
    BillRequestInput r2 = { "Gas", 1002, 300.00 };
    BillRequestInput out;
    bill_queue_init(&q);

    bill_queue_enqueue(&q, &r1);
    bill_queue_dequeue(&q, &out);
    TEST_ASSERT_TRUE(bill_queue_is_empty(&q));

    TEST_ASSERT_EQUAL_INT(STATUS_OK, bill_queue_enqueue(&q, &r2));
    TEST_ASSERT_FALSE(bill_queue_is_empty(&q));
    TEST_ASSERT_EQUAL_INT(STATUS_OK, bill_queue_dequeue(&q, &out));
    TEST_ASSERT_EQUAL_STRING("Gas", out.bill_type);
    TEST_ASSERT_TRUE(bill_queue_is_empty(&q));
}

/* UT-031: enqueue with NULL queue. */
void test_UT_031_enqueue_null_queue(void)
{
    BillRequestInput r1 = { "Electricity", 1001, 500.00 };
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, bill_queue_enqueue(NULL, &r1));
}

/* UT-032: enqueue with NULL input. */
void test_UT_032_enqueue_null_input(void)
{
    BillQueue q;
    bill_queue_init(&q);
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, bill_queue_enqueue(&q, NULL));
}

/* UT-033: dequeue with NULL queue. */
void test_UT_033_dequeue_null_queue(void)
{
    BillRequestInput out;
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, bill_queue_dequeue(NULL, &out));
}

/* UT-034: peek_front with NULL queue or NULL output. */
void test_UT_034_peek_front_null_safety(void)
{
    BillQueue q;
    BillRequestInput out;
    bill_queue_init(&q);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, bill_queue_peek_front(NULL, &out));
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, bill_queue_peek_front(&q, NULL));
}

/* UT-035: get_front_node with NULL queue. */
void test_UT_035_get_front_node_null(void)
{
    TEST_ASSERT_NULL(bill_queue_get_front_node(NULL));
}

/* UT-036: get_next_node with NULL node. */
void test_UT_036_get_next_node_null(void)
{
    TEST_ASSERT_NULL(bill_queue_get_next_node(NULL));
}

/* UT-037: destroy with remaining nodes. */
void test_UT_037_destroy_with_remaining_nodes(void)
{
    BillQueue q;
    BillRequestInput r1 = { "Electricity", 1001, 500.00 };
    BillRequestInput r2 = { "Gas", 1002, 300.00 };
    bill_queue_init(&q);

    bill_queue_enqueue(&q, &r1);
    bill_queue_enqueue(&q, &r2);

    bill_queue_destroy(&q);
    TEST_ASSERT_TRUE(bill_queue_is_empty(&q));
}

/* UT-038: destroy with NULL queue. */
void test_UT_038_destroy_null_safety(void)
{
    bill_queue_destroy(NULL);
    TEST_PASS();
}

/* UT-039: init with NULL queue. */
void test_UT_039_init_null_safety(void)
{
    bill_queue_init(NULL);
    TEST_PASS();
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_UT_022_init_then_empty);
    RUN_TEST(test_UT_023_dequeue_empty);
    RUN_TEST(test_UT_024_peek_front_empty);
    RUN_TEST(test_UT_025_enqueue_single);
    RUN_TEST(test_UT_026_peek_front_does_not_remove);
    RUN_TEST(test_UT_027_fifo_order_via_iteration);
    RUN_TEST(test_UT_028_fifo_order_via_dequeue);
    RUN_TEST(test_UT_029_dequeue_after_full_drain);
    RUN_TEST(test_UT_030_reenqueue_after_full_drain);
    RUN_TEST(test_UT_031_enqueue_null_queue);
    RUN_TEST(test_UT_032_enqueue_null_input);
    RUN_TEST(test_UT_033_dequeue_null_queue);
    RUN_TEST(test_UT_034_peek_front_null_safety);
    RUN_TEST(test_UT_035_get_front_node_null);
    RUN_TEST(test_UT_036_get_next_node_null);
    RUN_TEST(test_UT_037_destroy_with_remaining_nodes);
    RUN_TEST(test_UT_038_destroy_null_safety);
    RUN_TEST(test_UT_039_init_null_safety);

    return UNITY_END();
}
