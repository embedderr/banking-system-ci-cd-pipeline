/**
 * @file test_account.c
 * @brief Unity unit tests for the account module.
 *
 * Implements test cases UT-053 through UT-100 as specified in
 * docs/testing/unit_test_cases.xlsx. This module has the largest test
 * suite because it carries the most business logic and the most bugs
 * fixed during the C++ to C conversion (see design/LLD.md Section 7.3).
 */

#include "unity.h"
#include "account.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static NewAccountInput make_valid_input(const char *username, const char *password,
                                         bool wants_card, int pin)
{
    NewAccountInput input;
    memset(&input, 0, sizeof(input));
    strcpy(input.first_name, "Jane");
    strcpy(input.last_name, "Doe");
    strcpy(input.email, "jane@example.com");
    address_init(&input.home_address);
    input.phone_number = 9876543210LL;
    input.aadhaar_number = 123456789012LL;
    input.wants_debit_card = wants_card;
    input.debit_card_pin = pin;
    input.wants_cheque_book = true;
    strcpy(input.username, username);
    strcpy(input.password, password);
    return input;
}

/* ------------------------------------------------------------------- */
/* account_db_insert_pending                                            */
/* ------------------------------------------------------------------- */

/* UT-053: insert_pending valid -> STATUS_OK, PENDING, balance 0.00. */
void test_UT_053_insert_pending_valid(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_insert_pending(&db, &in));

    const AccountNode *node = account_db_get_first_pending(&db);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(ACCOUNT_STATUS_PENDING, node->status);
    TEST_ASSERT_EQUAL_DOUBLE(0.00, node->balance);

    account_db_destroy(&db);
}

/* UT-054: insert_pending with duplicate username -> STATUS_ERROR_DUPLICATE. */
void test_UT_054_insert_pending_duplicate_username(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in1 = make_valid_input("alice", "pass123", true, 1234);
    NewAccountInput in2 = make_valid_input("alice", "other456", true, 5678);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_insert_pending(&db, &in1));
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_DUPLICATE, account_db_insert_pending(&db, &in2));

    account_db_destroy(&db);
}

/* UT-055: insert_pending with an out-of-range PIN when a card is requested. */
void test_UT_055_insert_pending_invalid_pin_range(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("carol", "pass789", true, 99999);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_insert_pending(&db, &in));

    account_db_destroy(&db);
}

/* UT-056: insert_pending with a NULL database. */
void test_UT_056_insert_pending_null_db(void)
{
    NewAccountInput in = make_valid_input("dave", "pass000", false, 0);
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_insert_pending(NULL, &in));
}

/* UT-057: insert_pending with a NULL input. */
void test_UT_057_insert_pending_null_input(void)
{
    AccountDatabase db;
    account_db_init(&db);
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_insert_pending(&db, NULL));
    account_db_destroy(&db);
}

/* ------------------------------------------------------------------- */
/* account_db_username_exists                                           */
/* ------------------------------------------------------------------- */

/* UT-058: username_exists true and false cases. */
void test_UT_058_username_exists(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);

    TEST_ASSERT_TRUE(account_db_username_exists(&db, "alice"));
    TEST_ASSERT_FALSE(account_db_username_exists(&db, "nonexistent"));

    account_db_destroy(&db);
}

/* ------------------------------------------------------------------- */
/* account_db_get_first_pending / account_db_get_next_pending           */
/* ------------------------------------------------------------------- */

/* UT-059: iteration over pending accounts, in insertion order. */
void test_UT_059_pending_iteration_order(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in1 = make_valid_input("alice", "pass123", true, 1234);
    NewAccountInput in2 = make_valid_input("bob", "pass456", true, 5678);
    account_db_insert_pending(&db, &in1);
    account_db_insert_pending(&db, &in2);

    const AccountNode *p = account_db_get_first_pending(&db);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_STRING("alice", p->details.username);

    p = account_db_get_next_pending(p);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_STRING("bob", p->details.username);

    p = account_db_get_next_pending(p);
    TEST_ASSERT_NULL(p);

    account_db_destroy(&db);
}

/* UT-060: get_first_pending on an empty database. */
void test_UT_060_get_first_pending_empty_db(void)
{
    AccountDatabase db;
    account_db_init(&db);
    TEST_ASSERT_NULL(account_db_get_first_pending(&db));
    account_db_destroy(&db);
}

/* ------------------------------------------------------------------- */
/* account_db_approve                                                   */
/* ------------------------------------------------------------------- */

/* UT-061: Bug-regression -- first-ever approval gets the base account
   number (1007750), not base+1. The original prototype set status to
   Completed BEFORE calling generateAccountNumber(), causing it to count
   itself. */
void test_UT_061_approve_first_account_gets_base_number(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);

    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_approve(&db, node));
    TEST_ASSERT_EQUAL_INT(1007750L, node->account_number);

    account_db_destroy(&db);
}

/* UT-062: second approval gets base+1. */
void test_UT_062_approve_second_account_gets_base_plus_one(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in1 = make_valid_input("alice", "pass123", true, 1234);
    NewAccountInput in2 = make_valid_input("bob", "pass456", true, 5678);
    account_db_insert_pending(&db, &in1);
    account_db_insert_pending(&db, &in2);

    AccountNode *alice = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, alice);

    AccountNode *bob = (AccountNode *)account_db_get_first_pending(&db);
    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_approve(&db, bob));
    TEST_ASSERT_EQUAL_INT(1007751L, bob->account_number);

    account_db_destroy(&db);
}

/* UT-063: approve sets status to COMPLETED. */
void test_UT_063_approve_sets_status_completed(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);

    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);
    TEST_ASSERT_EQUAL_INT(ACCOUNT_STATUS_COMPLETED, node->status);

    account_db_destroy(&db);
}

/* UT-064: approve NULL safety. */
void test_UT_064_approve_null_safety(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_approve(NULL, node));
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_approve(&db, NULL));

    account_db_destroy(&db);
}

/* ------------------------------------------------------------------- */
/* account_db_decline                                                    */
/* ------------------------------------------------------------------- */

/* UT-065: Bug-regression -- decline actually sets status to DECLINED.
   The original prototype used "==" instead of "=" here, so the status
   was compared, not assigned, and never actually changed. */
void test_UT_065_decline_sets_status_declined(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("dave", "passdave", false, 0);
    account_db_insert_pending(&db, &in);

    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_decline(node));
    TEST_ASSERT_EQUAL_INT(ACCOUNT_STATUS_DECLINED, node->status);

    account_db_destroy(&db);
}

/* UT-066: decline NULL safety. */
void test_UT_066_decline_null_safety(void)
{
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_decline(NULL));
}

/* ------------------------------------------------------------------- */
/* account_db_delete                                                    */
/* ------------------------------------------------------------------- */

/* UT-067: delete a completed account by number. */
void test_UT_067_delete_completed_account(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);
    long acc_num = node->account_number;

    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_delete(&db, acc_num));
    TEST_ASSERT_NULL(account_db_find_by_credentials(&db, "alice", "pass123"));

    account_db_destroy(&db);
}

/* UT-068: Bug-regression -- delete the HEAD node; remaining accounts
   must all still be reachable. */
void test_UT_068_delete_head_preserves_list_integrity(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in1 = make_valid_input("alice", "pass123", true, 1234);
    NewAccountInput in2 = make_valid_input("bob", "pass456", true, 5678);
    account_db_insert_pending(&db, &in1);
    account_db_insert_pending(&db, &in2);

    AccountNode *alice = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, alice); /* 1007750 */
    AccountNode *bob = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, bob);   /* 1007751 */

    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_delete(&db, 1007750L));

    int count = 0;
    const AccountNode *n = account_db_get_all_first(&db);
    while (n != NULL) { count++; n = account_db_get_all_next(n); }
    TEST_ASSERT_EQUAL_INT(1, count);

    account_db_destroy(&db);
}

/* UT-069: Bug-regression -- delete the TAIL node; tail pointer must be
   correctly updated (original prototype never updated tail on delete). */
void test_UT_069_delete_tail_updates_tail_pointer(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in1 = make_valid_input("alice", "pass123", true, 1234);
    NewAccountInput in2 = make_valid_input("bob", "pass456", true, 5678);
    account_db_insert_pending(&db, &in1);
    account_db_insert_pending(&db, &in2);

    AccountNode *alice = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, alice); /* 1007750 */
    AccountNode *bob = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, bob);   /* 1007751, currently the tail */

    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_delete(&db, 1007751L));

    /* If tail wasn't correctly updated, inserting again could corrupt
       the list. Insert a new pending account and confirm the database
       is still fully consistent. */
    NewAccountInput in3 = make_valid_input("carol", "pass789", false, 0);
    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_insert_pending(&db, &in3));

    int count = 0;
    const AccountNode *n = account_db_get_all_first(&db);
    while (n != NULL) { count++; n = account_db_get_all_next(n); }
    TEST_ASSERT_EQUAL_INT(2, count); /* alice + carol */

    account_db_destroy(&db);
}

/* UT-070: Bug-regression -- deleting a not-found account number after a
   prior head-deletion must not crash (original dereferenced NULL here). */
void test_UT_070_delete_not_found_no_crash(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in1 = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in1);
    AccountNode *alice = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, alice); /* 1007750 */

    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_delete(&db, 1007750L));
    /* List is now empty. Deleting a nonexistent number must not crash. */
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_NOT_FOUND, account_db_delete(&db, 424242L));

    account_db_destroy(&db);
}

/* UT-071: Bug-regression -- account number 0 (default for PENDING
   accounts) must not be deletable; only COMPLETED accounts are eligible. */
void test_UT_071_delete_excludes_pending_accounts(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("dave", "passdave", false, 0);
    account_db_insert_pending(&db, &in); /* still PENDING, account_number == 0 */

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_NOT_FOUND, account_db_delete(&db, 0L));

    account_db_destroy(&db);
}

/* UT-072: delete with a NULL database. */
void test_UT_072_delete_null_db(void)
{
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_delete(NULL, 1007750L));
}

/* ------------------------------------------------------------------- */
/* account_db_find_by_credentials                                       */
/* ------------------------------------------------------------------- */

/* UT-073: find_by_credentials positive match. */
void test_UT_073_find_by_credentials_positive(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);

    AccountNode *found = account_db_find_by_credentials(&db, "alice", "pass123");
    TEST_ASSERT_NOT_NULL(found);
    TEST_ASSERT_EQUAL_INT(1007750L, found->account_number);

    account_db_destroy(&db);
}

/* UT-074: find_by_credentials with wrong password. */
void test_UT_074_find_by_credentials_wrong_password(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);

    TEST_ASSERT_NULL(account_db_find_by_credentials(&db, "alice", "wrongpass"));

    account_db_destroy(&db);
}

/* UT-075: find_by_credentials must not match a DECLINED account. */
void test_UT_075_find_by_credentials_excludes_declined(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("dave", "passdave", false, 0);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_decline(node);

    TEST_ASSERT_NULL(account_db_find_by_credentials(&db, "dave", "passdave"));

    account_db_destroy(&db);
}

/* UT-076: find_by_credentials NULL safety. */
void test_UT_076_find_by_credentials_null_safety(void)
{
    AccountDatabase db;
    account_db_init(&db);
    TEST_ASSERT_NULL(account_db_find_by_credentials(NULL, "alice", "pass123"));
    TEST_ASSERT_NULL(account_db_find_by_credentials(&db, NULL, "pass123"));
    TEST_ASSERT_NULL(account_db_find_by_credentials(&db, "alice", NULL));
    account_db_destroy(&db);
}

/* ------------------------------------------------------------------- */
/* account_db_check_atm_credentials                                      */
/* ------------------------------------------------------------------- */

/* UT-077: check_atm_credentials positive match. */
void test_UT_077_check_atm_credentials_positive(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);

    TEST_ASSERT_TRUE(account_db_check_atm_credentials(&db, 1007750L, 1234));

    account_db_destroy(&db);
}

/* UT-078: check_atm_credentials with wrong PIN or wrong account number. */
void test_UT_078_check_atm_credentials_negative(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);

    TEST_ASSERT_FALSE(account_db_check_atm_credentials(&db, 1007750L, 9999));
    TEST_ASSERT_FALSE(account_db_check_atm_credentials(&db, 9999999L, 1234));

    account_db_destroy(&db);
}

/* UT-079: check_atm_credentials NULL safety. */
void test_UT_079_check_atm_credentials_null_safety(void)
{
    TEST_ASSERT_FALSE(account_db_check_atm_credentials(NULL, 1007750L, 1234));
}

/* ------------------------------------------------------------------- */
/* account_db_find_by_atm_credentials                                    */
/* ------------------------------------------------------------------- */

/* UT-080: find_by_atm_credentials returns a usable pointer. */
void test_UT_080_find_by_atm_credentials_usable_pointer(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("eve", "evepass", true, 4242);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);

    AccountNode *found = account_db_find_by_atm_credentials(&db, node->account_number, 4242);
    TEST_ASSERT_NOT_NULL(found);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_apply_deposit(found, 100.00));
    Money bal;
    account_db_get_balance(found, &bal);
    TEST_ASSERT_EQUAL_DOUBLE(100.00, bal);

    account_db_destroy(&db);
}

/* UT-081: find_by_atm_credentials with wrong PIN or account number. */
void test_UT_081_find_by_atm_credentials_negative(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("eve", "evepass", true, 4242);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);
    long acc_num = node->account_number;

    TEST_ASSERT_NULL(account_db_find_by_atm_credentials(&db, acc_num, 9999));
    TEST_ASSERT_NULL(account_db_find_by_atm_credentials(&db, 999999L, 4242));

    account_db_destroy(&db);
}

/* ------------------------------------------------------------------- */
/* account_db_find_by_account_number                                     */
/* ------------------------------------------------------------------- */

/* UT-082: find_by_account_number matches regardless of PIN. */
void test_UT_082_find_by_account_number_positive(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);

    AccountNode *found = account_db_find_by_account_number(&db, node->account_number);
    TEST_ASSERT_NOT_NULL(found);
    TEST_ASSERT_EQUAL_STRING("alice", found->details.username);

    account_db_destroy(&db);
}

/* UT-083: find_by_account_number with a non-existent number. */
void test_UT_083_find_by_account_number_not_found(void)
{
    AccountDatabase db;
    account_db_init(&db);
    TEST_ASSERT_NULL(account_db_find_by_account_number(&db, 424242L));
    account_db_destroy(&db);
}

/* ------------------------------------------------------------------- */
/* account_db_apply_withdrawal                                           */
/* ------------------------------------------------------------------- */

/* UT-084: withdrawal success, balance decreases correctly. */
void test_UT_084_apply_withdrawal_success(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);
    account_db_apply_deposit(node, 500.00);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_apply_withdrawal(node, 200.00));
    Money bal;
    account_db_get_balance(node, &bal);
    TEST_ASSERT_EQUAL_DOUBLE(300.00, bal);

    account_db_destroy(&db);
}

/* UT-085: withdrawal exceeding balance leaves balance unchanged. */
void test_UT_085_apply_withdrawal_insufficient_funds(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);
    account_db_apply_deposit(node, 300.00);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INSUFFICIENT_FUNDS, account_db_apply_withdrawal(node, 999999.00));
    Money bal;
    account_db_get_balance(node, &bal);
    TEST_ASSERT_EQUAL_DOUBLE(300.00, bal);

    account_db_destroy(&db);
}

/* UT-086: negative withdrawal amount rejected. */
void test_UT_086_apply_withdrawal_negative_amount(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_apply_withdrawal(node, -10.00));

    account_db_destroy(&db);
}

/* UT-087: apply_withdrawal NULL safety. */
void test_UT_087_apply_withdrawal_null_safety(void)
{
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_apply_withdrawal(NULL, 10.00));
}

/* ------------------------------------------------------------------- */
/* account_db_apply_deposit                                              */
/* ------------------------------------------------------------------- */

/* UT-088: deposit success, balance increases correctly. */
void test_UT_088_apply_deposit_success(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_apply_deposit(node, 500.00));
    Money bal;
    account_db_get_balance(node, &bal);
    TEST_ASSERT_EQUAL_DOUBLE(500.00, bal);

    account_db_destroy(&db);
}

/* UT-089: negative deposit amount rejected. */
void test_UT_089_apply_deposit_negative_amount(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_apply_deposit(node, -10.00));

    account_db_destroy(&db);
}

/* UT-090: apply_deposit NULL safety. */
void test_UT_090_apply_deposit_null_safety(void)
{
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_apply_deposit(NULL, 10.00));
}

/* ------------------------------------------------------------------- */
/* account_db_change_pin                                                */
/* ------------------------------------------------------------------- */

/* UT-091: change PIN succeeds with correct old PIN and valid new PIN. */
void test_UT_091_change_pin_success(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);

    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_change_pin(node, 1234, 4321));
    TEST_ASSERT_EQUAL_INT(4321, node->details.debit_card_pin);

    account_db_destroy(&db);
}

/* UT-092: change PIN with an incorrect old PIN. */
void test_UT_092_change_pin_wrong_old_pin(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_change_pin(node, 9999, 4321));
    TEST_ASSERT_EQUAL_INT(1234, node->details.debit_card_pin);

    account_db_destroy(&db);
}

/* UT-093: change PIN with new PIN > 9999. */
void test_UT_093_change_pin_new_pin_too_large(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_change_pin(node, 1234, 99999));
    TEST_ASSERT_EQUAL_INT(1234, node->details.debit_card_pin);

    account_db_destroy(&db);
}

/* UT-094: change PIN with a negative new PIN. */
void test_UT_094_change_pin_negative_new_pin(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_change_pin(node, 1234, -1));
    TEST_ASSERT_EQUAL_INT(1234, node->details.debit_card_pin);

    account_db_destroy(&db);
}

/* UT-095: change_pin NULL safety. */
void test_UT_095_change_pin_null_safety(void)
{
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_change_pin(NULL, 1234, 4321));
}

/* ------------------------------------------------------------------- */
/* account_db_get_balance                                               */
/* ------------------------------------------------------------------- */

/* UT-096: get_balance returns the correct value. */
void test_UT_096_get_balance_positive(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    account_db_approve(&db, node);
    account_db_apply_deposit(node, 777.77);

    Money bal;
    TEST_ASSERT_EQUAL_INT(STATUS_OK, account_db_get_balance(node, &bal));
    TEST_ASSERT_EQUAL_DOUBLE(777.77, bal);

    account_db_destroy(&db);
}

/* UT-097: get_balance NULL safety. */
void test_UT_097_get_balance_null_safety(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in = make_valid_input("alice", "pass123", true, 1234);
    account_db_insert_pending(&db, &in);
    AccountNode *node = (AccountNode *)account_db_get_first_pending(&db);
    Money bal;

    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_get_balance(NULL, &bal));
    TEST_ASSERT_EQUAL_INT(STATUS_ERROR_INVALID_INPUT, account_db_get_balance(node, NULL));

    account_db_destroy(&db);
}

/* ------------------------------------------------------------------- */
/* account_db_get_all_first / account_db_get_all_next                   */
/* ------------------------------------------------------------------- */

/* UT-098: iteration over ALL accounts regardless of status. */
void test_UT_098_get_all_iteration_regardless_of_status(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in1 = make_valid_input("alice", "pass123", true, 1234); /* will stay PENDING */
    NewAccountInput in2 = make_valid_input("bob", "pass456", true, 5678);   /* will be COMPLETED */
    NewAccountInput in3 = make_valid_input("dave", "passdave", false, 0);  /* will be DECLINED */

    account_db_insert_pending(&db, &in1);
    account_db_insert_pending(&db, &in2);
    account_db_insert_pending(&db, &in3);

    AccountNode *first = (AccountNode *)account_db_get_all_first(&db);
    AccountNode *second = (AccountNode *)account_db_get_all_next(first);
    AccountNode *third = (AccountNode *)account_db_get_all_next(second);

    account_db_approve(&db, second);
    account_db_decline(third);

    int count = 0;
    const AccountNode *n = account_db_get_all_first(&db);
    while (n != NULL) { count++; n = account_db_get_all_next(n); }
    TEST_ASSERT_EQUAL_INT(3, count);

    account_db_destroy(&db);
}

/* ------------------------------------------------------------------- */
/* account_db_destroy / account_db_init                                 */
/* ------------------------------------------------------------------- */

/* UT-099: destroy frees all records. */
void test_UT_099_destroy_frees_all_records(void)
{
    AccountDatabase db;
    account_db_init(&db);
    NewAccountInput in1 = make_valid_input("alice", "pass123", true, 1234);
    NewAccountInput in2 = make_valid_input("bob", "pass456", true, 5678);
    account_db_insert_pending(&db, &in1);
    account_db_insert_pending(&db, &in2);

    account_db_destroy(&db);
    TEST_ASSERT_NULL(account_db_get_all_first(&db));
}

/* UT-100: init/destroy NULL safety. */
void test_UT_100_init_destroy_null_safety(void)
{
    account_db_init(NULL);
    account_db_destroy(NULL);
    TEST_PASS();
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_UT_053_insert_pending_valid);
    RUN_TEST(test_UT_054_insert_pending_duplicate_username);
    RUN_TEST(test_UT_055_insert_pending_invalid_pin_range);
    RUN_TEST(test_UT_056_insert_pending_null_db);
    RUN_TEST(test_UT_057_insert_pending_null_input);
    RUN_TEST(test_UT_058_username_exists);
    RUN_TEST(test_UT_059_pending_iteration_order);
    RUN_TEST(test_UT_060_get_first_pending_empty_db);
    RUN_TEST(test_UT_061_approve_first_account_gets_base_number);
    RUN_TEST(test_UT_062_approve_second_account_gets_base_plus_one);
    RUN_TEST(test_UT_063_approve_sets_status_completed);
    RUN_TEST(test_UT_064_approve_null_safety);
    RUN_TEST(test_UT_065_decline_sets_status_declined);
    RUN_TEST(test_UT_066_decline_null_safety);
    RUN_TEST(test_UT_067_delete_completed_account);
    RUN_TEST(test_UT_068_delete_head_preserves_list_integrity);
    RUN_TEST(test_UT_069_delete_tail_updates_tail_pointer);
    RUN_TEST(test_UT_070_delete_not_found_no_crash);
    RUN_TEST(test_UT_071_delete_excludes_pending_accounts);
    RUN_TEST(test_UT_072_delete_null_db);
    RUN_TEST(test_UT_073_find_by_credentials_positive);
    RUN_TEST(test_UT_074_find_by_credentials_wrong_password);
    RUN_TEST(test_UT_075_find_by_credentials_excludes_declined);
    RUN_TEST(test_UT_076_find_by_credentials_null_safety);
    RUN_TEST(test_UT_077_check_atm_credentials_positive);
    RUN_TEST(test_UT_078_check_atm_credentials_negative);
    RUN_TEST(test_UT_079_check_atm_credentials_null_safety);
    RUN_TEST(test_UT_080_find_by_atm_credentials_usable_pointer);
    RUN_TEST(test_UT_081_find_by_atm_credentials_negative);
    RUN_TEST(test_UT_082_find_by_account_number_positive);
    RUN_TEST(test_UT_083_find_by_account_number_not_found);
    RUN_TEST(test_UT_084_apply_withdrawal_success);
    RUN_TEST(test_UT_085_apply_withdrawal_insufficient_funds);
    RUN_TEST(test_UT_086_apply_withdrawal_negative_amount);
    RUN_TEST(test_UT_087_apply_withdrawal_null_safety);
    RUN_TEST(test_UT_088_apply_deposit_success);
    RUN_TEST(test_UT_089_apply_deposit_negative_amount);
    RUN_TEST(test_UT_090_apply_deposit_null_safety);
    RUN_TEST(test_UT_091_change_pin_success);
    RUN_TEST(test_UT_092_change_pin_wrong_old_pin);
    RUN_TEST(test_UT_093_change_pin_new_pin_too_large);
    RUN_TEST(test_UT_094_change_pin_negative_new_pin);
    RUN_TEST(test_UT_095_change_pin_null_safety);
    RUN_TEST(test_UT_096_get_balance_positive);
    RUN_TEST(test_UT_097_get_balance_null_safety);
    RUN_TEST(test_UT_098_get_all_iteration_regardless_of_status);
    RUN_TEST(test_UT_099_destroy_frees_all_records);
    RUN_TEST(test_UT_100_init_destroy_null_safety);

    return UNITY_END();
}
