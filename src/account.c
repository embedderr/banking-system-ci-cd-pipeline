/**
 * @file account.c
 * @brief Implementation of the account database module.
 */

#include "account.h"

#include <stdlib.h>
#include <string.h>

/** Base value for generated account numbers, matching the original scheme. */
#define ACCOUNT_NUMBER_BASE 1007750L

/** Inclusive bounds for a valid 4-digit debit card PIN. */
#define PIN_MIN 0
#define PIN_MAX 9999

void account_db_init(AccountDatabase *db) {
    if (db == NULL) {
        return;
    }

    db->head = NULL;
    db->tail = NULL;
}

void account_db_destroy(AccountDatabase *db) {
    AccountNode *current;
    AccountNode *next_node;

    if (db == NULL) {
        return;
    }

    current = db->head;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }

    db->head = NULL;
    db->tail = NULL;
}

bool account_db_username_exists(const AccountDatabase *db, const char *username) {
    const AccountNode *current;

    if ((db == NULL) || (username == NULL)) {
        return false;
    }

    current = db->head;
    while (current != NULL) {
        if (strcmp(current->details.username, username) == 0) {
            return true;
        }
        current = current->next;
    }

    return false;
}

StatusCode account_db_insert_pending(AccountDatabase *db, const NewAccountInput *input) {
    AccountNode *new_node;

    if ((db == NULL) || (input == NULL)) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    if (account_db_username_exists(db, input->username)) {
        return STATUS_ERROR_DUPLICATE;
    }

    if (input->wants_debit_card &&
        ((input->debit_card_pin < PIN_MIN) || (input->debit_card_pin > PIN_MAX))) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    new_node = (AccountNode *)malloc(sizeof(AccountNode));
    if (new_node == NULL) {
        return STATUS_ERROR_ALLOC_FAILED;
    }

    new_node->details = *input;
    new_node->status = ACCOUNT_STATUS_PENDING;
    new_node->balance = 0.00;
    new_node->account_number = 0L;
    new_node->next = NULL;

    if (db->head == NULL) {
        new_node->prev = NULL;
        db->head = new_node;
        db->tail = new_node;
    } else {
        new_node->prev = db->tail;
        db->tail->next = new_node;
        db->tail = new_node;
    }

    return STATUS_OK;
}

const AccountNode *account_db_get_first_pending(const AccountDatabase *db) {
    const AccountNode *current;

    if (db == NULL) {
        return NULL;
    }

    current = db->head;
    while (current != NULL) {
        if (current->status == ACCOUNT_STATUS_PENDING) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

const AccountNode *account_db_get_next_pending(const AccountNode *current) {
    const AccountNode *node;

    if (current == NULL) {
        return NULL;
    }

    node = current->next;
    while (node != NULL) {
        if (node->status == ACCOUNT_STATUS_PENDING) {
            return node;
        }
        node = node->next;
    }

    return NULL;
}

long account_db_generate_account_number(const AccountDatabase *db) {
    const AccountNode *current;
    long completed_count = 0L;

    if (db == NULL) {
        return 0L;
    }

    current = db->head;
    while (current != NULL) {
        if (current->status == ACCOUNT_STATUS_COMPLETED) {
            completed_count++;
        }
        current = current->next;
    }

    return ACCOUNT_NUMBER_BASE + completed_count;
}

StatusCode account_db_approve(AccountDatabase *db, AccountNode *node) {
    long new_account_number;

    if ((db == NULL) || (node == NULL)) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    /* Computed BEFORE the status change below, so this node is not
       counted among "already completed" accounts (fixes an off-by-one
       present in the original prototype, where the status was set to
       Completed before generateAccountNumber() was called, causing it
       to count itself). */
    new_account_number = account_db_generate_account_number(db);

    node->account_number = new_account_number;
    node->status = ACCOUNT_STATUS_COMPLETED;

    return STATUS_OK;
}

StatusCode account_db_decline(AccountNode *node) {
    if (node == NULL) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    node->status = ACCOUNT_STATUS_DECLINED;

    return STATUS_OK;
}

StatusCode account_db_delete(AccountDatabase *db, long account_number) {
    AccountNode *current;

    if (db == NULL) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    current = db->head;
    while (current != NULL) {
        if ((current->status == ACCOUNT_STATUS_COMPLETED) &&
            (current->account_number == account_number)) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                db->head = current->next;
            }

            if (current->next != NULL) {
                current->next->prev = current->prev;
            } else {
                db->tail = current->prev;
            }

            free(current);
            return STATUS_OK;
        }
        current = current->next;
    }

    return STATUS_ERROR_NOT_FOUND;
}

AccountNode *account_db_find_by_credentials(AccountDatabase *db, const char *username,
                                            const char *password) {
    AccountNode *current;

    if ((db == NULL) || (username == NULL) || (password == NULL)) {
        return NULL;
    }

    current = db->head;
    while (current != NULL) {
        if ((current->status == ACCOUNT_STATUS_COMPLETED) &&
            (strcmp(current->details.username, username) == 0) &&
            (strcmp(current->details.password, password) == 0)) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

bool account_db_check_atm_credentials(const AccountDatabase *db, long account_number, int pin) {
    const AccountNode *current;

    if (db == NULL) {
        return false;
    }

    current = db->head;
    while (current != NULL) {
        if ((current->status == ACCOUNT_STATUS_COMPLETED) &&
            (current->account_number == account_number) &&
            (current->details.debit_card_pin == pin)) {
            return true;
        }
        current = current->next;
    }

    return false;
}

AccountNode *account_db_find_by_atm_credentials(AccountDatabase *db, long account_number, int pin) {
    AccountNode *current;

    if (db == NULL) {
        return NULL;
    }

    current = db->head;
    while (current != NULL) {
        if ((current->status == ACCOUNT_STATUS_COMPLETED) &&
            (current->account_number == account_number) &&
            (current->details.debit_card_pin == pin)) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

AccountNode *account_db_find_by_account_number(AccountDatabase *db, long account_number) {
    AccountNode *current;

    if (db == NULL) {
        return NULL;
    }

    current = db->head;
    while (current != NULL) {
        if ((current->status == ACCOUNT_STATUS_COMPLETED) &&
            (current->account_number == account_number)) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

StatusCode account_db_apply_withdrawal(AccountNode *acc, Money amount) {
    if ((acc == NULL) || (amount < 0.0)) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    if (amount > acc->balance) {
        return STATUS_ERROR_INSUFFICIENT_FUNDS;
    }

    acc->balance -= amount;

    return STATUS_OK;
}

StatusCode account_db_apply_deposit(AccountNode *acc, Money amount) {
    if ((acc == NULL) || (amount < 0.0)) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    acc->balance += amount;

    return STATUS_OK;
}

StatusCode account_db_change_pin(AccountNode *acc, int old_pin, int new_pin) {
    if (acc == NULL) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    if (acc->details.debit_card_pin != old_pin) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    if ((new_pin < PIN_MIN) || (new_pin > PIN_MAX)) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    acc->details.debit_card_pin = new_pin;

    return STATUS_OK;
}

StatusCode account_db_get_balance(const AccountNode *acc, Money *out_balance) {
    if ((acc == NULL) || (out_balance == NULL)) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    *out_balance = acc->balance;

    return STATUS_OK;
}

const AccountNode *account_db_get_all_first(const AccountDatabase *db) {
    if (db == NULL) {
        return NULL;
    }

    return db->head;
}

const AccountNode *account_db_get_all_next(const AccountNode *current) {
    if (current == NULL) {
        return NULL;
    }

    return current->next;
}
