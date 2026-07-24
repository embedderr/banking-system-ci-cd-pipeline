/**
 * @file account.h
 * @brief The account database: lifecycle (pending -> approved/declined),
 *        credential lookup, and balance/PIN operations.
 *
 * Per the I/O Separation Principle (docs/design/LLD.md, Section 1.2),
 * this module performs no console I/O. ui_screens collects a
 * NewAccountInput from the user and passes it to
 * account_db_insert_pending(); this module only validates, stores, and
 * returns data.
 *
 * Traceability: satisfies SRS-001 through SRS-014, SRS-031, SRS-032.
 */

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdbool.h>

#include "address.h"
#include "common.h"

/**
 * @brief Lifecycle state of an account request.
 */
typedef enum {
    ACCOUNT_STATUS_PENDING,   /**< Submitted, awaiting admin review. */
    ACCOUNT_STATUS_COMPLETED, /**< Approved; has a real account number and can transact. */
    ACCOUNT_STATUS_DECLINED   /**< Rejected by admin; cannot transact. */
} AccountStatus;

/**
 * @brief Raw data for a new account request, collected by ui_screens
 *        before being handed to account_db_insert_pending().
 */
typedef struct {
    char first_name[MAX_NAME_LEN];
    char last_name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    Address home_address;
    long long phone_number;
    long long aadhaar_number;
    bool wants_debit_card;
    int debit_card_pin; /**< Valid only if wants_debit_card == true; must be 0-9999. */
    bool wants_cheque_book;
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
} NewAccountInput;

/**
 * @brief One account record in the database's doubly linked list.
 */
typedef struct AccountNode {
    NewAccountInput details; /**< Data captured at request time. */
    AccountStatus status;    /**< Current lifecycle state. */
    Money balance;           /**< Current balance; 0.00 until approved and funded. */
    long account_number;     /**< Assigned only when status becomes COMPLETED; 0 until then. */
    struct AccountNode *next;
    struct AccountNode *prev;
} AccountNode;

/**
 * @brief The full collection of account records.
 *
 * Must be initialized with account_db_init() before use, and released
 * with account_db_destroy() when no longer needed.
 */
typedef struct {
    AccountNode *head;
    AccountNode *tail;
} AccountDatabase;

/**
 * @brief Initializes @p db to an empty database.
 *
 * @param db  Non-NULL pointer to the database to initialize.
 */
void account_db_init(AccountDatabase *db);

/**
 * @brief Frees every account record in @p db and resets it to empty.
 *
 * @param db  Non-NULL pointer to the database to destroy.
 */
void account_db_destroy(AccountDatabase *db);

/**
 * @brief Checks whether @p username is already used by any account
 *        record in @p db, regardless of that record's status.
 *
 * @param db        Non-NULL pointer to the database.
 * @param username  Non-NULL, null-terminated username to check.
 * @return true if the username is already taken; false otherwise, or if
 *         @p db or @p username is NULL.
 */
bool account_db_username_exists(const AccountDatabase *db, const char *username);

/**
 * @brief Submits a new account request with status ACCOUNT_STATUS_PENDING
 *        and an initial balance of 0.00.
 *
 * @param db     Non-NULL pointer to the database.
 * @param input  Non-NULL pointer to the request data to copy in.
 * @return STATUS_OK on success;
 *         STATUS_ERROR_DUPLICATE if the username is already taken;
 *         STATUS_ERROR_INVALID_INPUT if @p db/@p input is NULL, or if
 *         wants_debit_card is true and debit_card_pin is outside 0-9999;
 *         STATUS_ERROR_ALLOC_FAILED if memory allocation failed.
 */
StatusCode account_db_insert_pending(AccountDatabase *db, const NewAccountInput *input);

/**
 * @brief Returns the first account record with status
 *        ACCOUNT_STATUS_PENDING, for read-only iteration.
 *
 * @param db  Non-NULL pointer to the database.
 * @return Pointer to the first pending record, or NULL if none exist or
 *         @p db is NULL.
 */
const AccountNode *account_db_get_first_pending(const AccountDatabase *db);

/**
 * @brief Returns the next account record with status
 *        ACCOUNT_STATUS_PENDING after @p current, for read-only iteration.
 *
 * @param current  Pointer to the current node (as returned by
 *                 account_db_get_first_pending() or a previous call to
 *                 this function).
 * @return Pointer to the next pending record, or NULL if none remain or
 *         @p current is NULL.
 */
const AccountNode *account_db_get_next_pending(const AccountNode *current);

/**
 * @brief Approves a pending account request: assigns it a unique account
 *        number and sets its status to ACCOUNT_STATUS_COMPLETED.
 *
 * The new account number is computed before the status change is applied,
 * so the record being approved is not counted among existing completed
 * accounts (this avoids an off-by-one present in the original prototype).
 *
 * @param db    Non-NULL pointer to the database that owns @p node.
 * @param node  Non-NULL pointer to the pending record to approve.
 * @return STATUS_OK on success;
 *         STATUS_ERROR_INVALID_INPUT if @p db or @p node is NULL.
 */
StatusCode account_db_approve(AccountDatabase *db, AccountNode *node);

/**
 * @brief Declines a pending account request: sets its status to
 *        ACCOUNT_STATUS_DECLINED. No account number is assigned.
 *
 * @param node  Non-NULL pointer to the pending record to decline.
 * @return STATUS_OK on success;
 *         STATUS_ERROR_INVALID_INPUT if @p node is NULL.
 */
StatusCode account_db_decline(AccountNode *node);

/**
 * @brief Deletes a completed account by its account number.
 *
 * Only records with status ACCOUNT_STATUS_COMPLETED are eligible, since
 * pending/declined records do not yet have a unique, meaningful account
 * number (this avoids an ambiguity present in the original prototype,
 * where multiple pending records could all match account number 0).
 *
 * @param db              Non-NULL pointer to the database.
 * @param account_number  Account number to delete.
 * @return STATUS_OK on success;
 *         STATUS_ERROR_NOT_FOUND if no matching completed account exists;
 *         STATUS_ERROR_INVALID_INPUT if @p db is NULL.
 */
StatusCode account_db_delete(AccountDatabase *db, long account_number);

/**
 * @brief Finds a completed account by username and password.
 *
 * @param db        Non-NULL pointer to the database.
 * @param username  Non-NULL, null-terminated username to match.
 * @param password  Non-NULL, null-terminated password to match.
 * @return Pointer to the matching account record, or NULL if no match
 *         is found or any argument is NULL.
 */
AccountNode *account_db_find_by_credentials(AccountDatabase *db, const char *username,
                                            const char *password);

/**
 * @brief Checks whether @p account_number and @p pin match a completed
 *        account, for ATM login.
 *
 * @param db              Non-NULL pointer to the database.
 * @param account_number  Account number to match.
 * @param pin             Debit card PIN to match.
 * @return true if a completed account matches both values; false
 *         otherwise, or if @p db is NULL.
 */
bool account_db_check_atm_credentials(const AccountDatabase *db, long account_number, int pin);

/**
 * @brief Finds a completed account by account number and debit card PIN.
 *
 * Intended for use immediately after a successful ATM login
 * (account_db_check_atm_credentials() returned true), so the returned
 * pointer can be reused for every operation in the same ATM session
 * without re-asking the user for their username and password. The
 * original prototype re-authenticated via username/password for every
 * single ATM operation despite already having verified account number
 * and PIN at entry -- this function removes that inconsistency.
 *
 * @param db              Non-NULL pointer to the database.
 * @param account_number  Account number to match.
 * @param pin             Debit card PIN to match.
 * @return Pointer to the matching completed account, or NULL if no match
 *         is found or @p db is NULL.
 */
AccountNode *account_db_find_by_atm_credentials(AccountDatabase *db, long account_number, int pin);

/**
 * @brief Finds a completed account by account number alone, with no
 *        credential check.
 *
 * Intended for administrative operations where the caller (an
 * authenticated admin) already has authority to act on any account by
 * its number -- e.g. applying a queued cash-queue transaction (SRS-030),
 * which specifies only an account number, not a PIN or password.
 *
 * @param db              Non-NULL pointer to the database.
 * @param account_number  Account number to match.
 * @return Pointer to the matching completed account, or NULL if no match
 *         is found or @p db is NULL.
 */
AccountNode *account_db_find_by_account_number(AccountDatabase *db, long account_number);

/**
 * @brief Withdraws @p amount from @p acc's balance.
 *
 * @param acc     Non-NULL pointer to the account to debit.
 * @param amount  Amount to withdraw; must be >= 0.
 * @return STATUS_OK on success;
 *         STATUS_ERROR_INSUFFICIENT_FUNDS if @p amount exceeds the
 *         current balance (balance is left unchanged in this case);
 *         STATUS_ERROR_INVALID_INPUT if @p acc is NULL or @p amount < 0.
 */
StatusCode account_db_apply_withdrawal(AccountNode *acc, Money amount);

/**
 * @brief Deposits @p amount into @p acc's balance.
 *
 * @param acc     Non-NULL pointer to the account to credit.
 * @param amount  Amount to deposit; must be >= 0.
 * @return STATUS_OK on success;
 *         STATUS_ERROR_INVALID_INPUT if @p acc is NULL or @p amount < 0.
 */
StatusCode account_db_apply_deposit(AccountNode *acc, Money amount);

/**
 * @brief Changes @p acc's debit card PIN, after verifying the existing PIN.
 *
 * @param acc      Non-NULL pointer to the account to update.
 * @param old_pin  Must match the account's current PIN.
 * @param new_pin  Must be in the range 0-9999 inclusive.
 * @return STATUS_OK on success;
 *         STATUS_ERROR_INVALID_INPUT if @p acc is NULL, @p old_pin does
 *         not match the current PIN, or @p new_pin is outside 0-9999.
 */
StatusCode account_db_change_pin(AccountNode *acc, int old_pin, int new_pin);

/**
 * @brief Reads @p acc's current balance.
 *
 * @param acc          Non-NULL pointer to the account to read.
 * @param out_balance  Non-NULL pointer to receive the balance.
 * @return STATUS_OK on success;
 *         STATUS_ERROR_INVALID_INPUT if @p acc or @p out_balance is NULL.
 */
StatusCode account_db_get_balance(const AccountNode *acc, Money *out_balance);

/**
 * @brief Returns the first account record in @p db, regardless of status,
 *        for read-only iteration over the entire database.
 *
 * @param db  Non-NULL pointer to the database.
 * @return Pointer to the first record, or NULL if the database is empty
 *         or @p db is NULL.
 */
const AccountNode *account_db_get_all_first(const AccountDatabase *db);

/**
 * @brief Returns the record following @p current, regardless of status.
 *
 * @param current  Pointer to the current node.
 * @return Pointer to the next record, or NULL if @p current is the last
 *         node or is itself NULL.
 */
const AccountNode *account_db_get_all_next(const AccountNode *current);

/**
 * @brief Computes the next sequential account number, based on the count
 *        of currently completed accounts in @p db.
 *
 * @param db  Non-NULL pointer to the database.
 * @return The next account number to assign, or 0 if @p db is NULL.
 */
long account_db_generate_account_number(const AccountDatabase *db);

#endif /* ACCOUNT_H */
