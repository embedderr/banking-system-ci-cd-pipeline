/**
 * @file main.c
 * @brief Top-level orchestration for the Banking System.
 *
 * Every menu loop from the original prototype (which used labeled goto
 * statements) is replaced here with a structured do { ... } while (...)
 * loop. This file contains no business logic of its own -- it only
 * sequences calls between ui_screens (I/O) and the business-logic
 * modules (account, bill_queue, cash_queue, admin), per the I/O
 * Separation Principle (docs/design/LLD.md, Section 1.2).
 */

#include <stdio.h>

#include "account.h"
#include "address.h"
#include "admin.h"
#include "bill_queue.h"
#include "cash_queue.h"
#include "common.h"
#include "platform.h"
#include "ui_screens.h"

/* Top-level main menu options */
#define MAIN_MENU_ENTER_BANK 1
#define MAIN_MENU_ENTER_ATM 2
#define MAIN_MENU_ADMIN_LOGIN 3
#define MAIN_MENU_EXIT 4

/* Bank menu options */
#define BANK_MENU_PAY_BILLS 1
#define BANK_MENU_CASH_QUEUE 2
#define BANK_MENU_OPEN_ACCOUNT 3
#define BANK_MENU_VIEW_ACCOUNT 4
#define BANK_MENU_EXIT 5

/* ATM menu options */
#define ATM_MENU_WITHDRAW 1
#define ATM_MENU_BALANCE 2
#define ATM_MENU_CHANGE_PIN 3
#define ATM_MENU_EXIT 4

/* Admin menu options */
#define ADMIN_MENU_ACCOUNT_OPENINGS 1
#define ADMIN_MENU_BILL_PAYMENTS 2
#define ADMIN_MENU_CASH_TRANSACTIONS 3
#define ADMIN_MENU_EXIT 4

/* Admin account-openings sub-menu options */
#define ACC_OPEN_MENU_VIEW_PENDING 1
#define ACC_OPEN_MENU_PROCESS 2
#define ACC_OPEN_MENU_VIEW_ALL 3
#define ACC_OPEN_MENU_DELETE 4
#define ACC_OPEN_MENU_EXIT 5

/* Admin bill-payments sub-menu options */
#define BILL_ADMIN_MENU_VIEW_ALL 1
#define BILL_ADMIN_MENU_VIEW_FRONT 2
#define BILL_ADMIN_MENU_PROCESS_FRONT 3
#define BILL_ADMIN_MENU_EXIT 4

/* ------------------------------------------------------------------- */
/* Bank menu handlers                                                    */
/* ------------------------------------------------------------------- */

static void handle_pay_bills(BillQueue *bill_queue) {
    BillRequestInput input;

    platform_clear_screen();
    if (!ui_collect_bill_request(&input)) {
        ui_show_bill_enqueue_result(STATUS_ERROR_INVALID_INPUT);
        return;
    }

    ui_show_bill_enqueue_result(bill_queue_enqueue(bill_queue, &input));
}

static void handle_cash_queue_submit(CashQueue *cash_queue) {
    CashRequestInput input;

    platform_clear_screen();
    if (!ui_collect_cash_request(&input)) {
        ui_show_cash_enqueue_result(STATUS_ERROR_INVALID_INPUT);
        return;
    }

    ui_show_cash_enqueue_result(cash_queue_enqueue(cash_queue, &input));
}

static void handle_open_account(AccountDatabase *account_db) {
    NewAccountInput input;

    platform_clear_screen();
    if (!ui_collect_new_account_request(&input)) {
        ui_show_account_request_result(STATUS_ERROR_INVALID_INPUT);
        return;
    }

    ui_show_account_request_result(account_db_insert_pending(account_db, &input));
}

static void handle_view_account(AccountDatabase *account_db) {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];

    platform_clear_screen();
    if (!ui_collect_login_credentials(username, sizeof(username), password, sizeof(password))) {
        ui_show_account_details(NULL);
        return;
    }

    ui_show_account_details(account_db_find_by_credentials(account_db, username, password));
}

/* ------------------------------------------------------------------- */
/* ATM menu handlers                                                     */
/* ------------------------------------------------------------------- */

static void handle_atm_withdraw(AccountNode *acc) {
    Money amount;
    Money remaining = 0.0;
    StatusCode result;

    platform_clear_screen();
    if (!ui_collect_withdrawal_amount(&amount)) {
        ui_show_withdrawal_result(STATUS_ERROR_INVALID_INPUT, remaining);
        return;
    }

    result = account_db_apply_withdrawal(acc, amount);
    if (result == STATUS_OK) {
        (void)account_db_get_balance(acc, &remaining);
    }
    ui_show_withdrawal_result(result, remaining);
}

static void handle_atm_balance(AccountNode *acc) {
    Money balance = 0.0;

    platform_clear_screen();
    (void)account_db_get_balance(acc, &balance);
    ui_show_balance(acc->account_number, balance);
}

static void handle_atm_change_pin(AccountNode *acc) {
    int old_pin;
    int new_pin;

    platform_clear_screen();
    if (!ui_collect_pin_change(&old_pin, &new_pin)) {
        ui_show_pin_change_result(STATUS_ERROR_INVALID_INPUT);
        return;
    }

    ui_show_pin_change_result(account_db_change_pin(acc, old_pin, new_pin));
}

/* ------------------------------------------------------------------- */
/* Admin: account openings handlers                                      */
/* ------------------------------------------------------------------- */

static void handle_view_pending_accounts(const AccountDatabase *account_db) {
    const AccountNode *node;

    platform_clear_screen();
    node = account_db_get_first_pending(account_db);
    if (node == NULL) {
        ui_show_database_empty_message();
        return;
    }

    while (node != NULL) {
        ui_show_account_details(node);
        node = account_db_get_next_pending(node);
    }
    platform_wait_for_keypress();
}

static void handle_process_pending_accounts(AccountDatabase *account_db) {
    AccountNode *node;
    AccountNode *next_node;

    platform_clear_screen();
    node = (AccountNode *)account_db_get_first_pending(account_db);
    if (node == NULL) {
        ui_show_database_empty_message();
        return;
    }

    while (node != NULL) {
        bool approve;

        /* Capture the next pending node BEFORE this one's status changes,
           since account_db_get_next_pending() searches forward from the
           current node and would otherwise skip past what used to be
           "the next pending node" once this node is no longer pending. */
        next_node = (AccountNode *)account_db_get_next_pending(node);

        ui_show_account_details(node);
        approve = ui_confirm_yes_no("Approve this account?");

        if (approve) {
            ui_show_account_decision_result(account_db_approve(account_db, node), true);
        } else {
            ui_show_account_decision_result(account_db_decline(node), false);
        }

        node = next_node;
    }

    platform_wait_for_keypress();
}

static void handle_view_all_accounts(const AccountDatabase *account_db) {
    const AccountNode *node;

    platform_clear_screen();
    node = account_db_get_all_first(account_db);
    if (node == NULL) {
        ui_show_database_empty_message();
        return;
    }

    while (node != NULL) {
        ui_show_account_details(node);
        node = account_db_get_all_next(node);
    }
    platform_wait_for_keypress();
}

static void handle_delete_account(AccountDatabase *account_db) {
    long account_number;

    platform_clear_screen();
    if (!ui_collect_account_number(&account_number)) {
        ui_show_account_delete_result(STATUS_ERROR_INVALID_INPUT);
        return;
    }

    ui_show_account_delete_result(account_db_delete(account_db, account_number));
}

/* ------------------------------------------------------------------- */
/* Admin: bill payments handlers                                         */
/* ------------------------------------------------------------------- */

static void handle_view_bill_queue(const BillQueue *bill_queue) {
    const BillNode *node;
    int index = 0;

    platform_clear_screen();
    node = bill_queue_get_front_node(bill_queue);
    if (node == NULL) {
        ui_show_queue_empty_message("Bill Payment");
        return;
    }

    while (node != NULL) {
        ui_show_bill_request(&node->data, index);
        index++;
        node = bill_queue_get_next_node(node);
    }
    platform_wait_for_keypress();
}

static void handle_view_bill_front(const BillQueue *bill_queue) {
    BillRequestInput front;

    platform_clear_screen();
    if (bill_queue_peek_front(bill_queue, &front) != STATUS_OK) {
        ui_show_queue_empty_message("Bill Payment");
        return;
    }

    ui_show_bill_request(&front, 0);
    platform_wait_for_keypress();
}

static void handle_process_bill_front(BillQueue *bill_queue) {
    platform_clear_screen();
    ui_show_bill_process_result(bill_queue_dequeue(bill_queue, NULL));
}

/* ------------------------------------------------------------------- */
/* Admin: cash queue handlers (reachable via the same bill-payments-style
   sub-menu pattern; wired directly since the original prototype exposed
   no admin menu entry for this at all -- see LLD Section 5/6 discussion
   on completing the withdraw/deposit queue, SRS-029/SRS-030). Exposed
   here under the account-openings menu's sibling flows is avoided; a
   dedicated cash-queue admin path mirrors the bill-payments one. */

static void handle_view_cash_queue(const CashQueue *cash_queue) {
    const CashNode *node;
    int index = 0;

    platform_clear_screen();
    node = cash_queue_get_front_node(cash_queue);
    if (node == NULL) {
        ui_show_queue_empty_message("Cash Transaction");
        return;
    }

    while (node != NULL) {
        ui_show_cash_request(&node->data, index);
        index++;
        node = cash_queue_get_next_node(node);
    }
    platform_wait_for_keypress();
}

static void handle_view_cash_front(const CashQueue *cash_queue) {
    CashRequestInput front;

    platform_clear_screen();
    if (cash_queue_peek_front(cash_queue, &front) != STATUS_OK) {
        ui_show_queue_empty_message("Cash Transaction");
        return;
    }

    ui_show_cash_request(&front, 0);
    platform_wait_for_keypress();
}

static void handle_process_cash_front(CashQueue *cash_queue, AccountDatabase *account_db) {
    CashRequestInput request;
    StatusCode result;

    platform_clear_screen();
    result = cash_queue_dequeue(cash_queue, &request);
    if (result != STATUS_OK) {
        ui_show_cash_process_result(result);
        return;
    }

    AccountNode *target = account_db_find_by_account_number(account_db, request.account_number);
    if (target == NULL) {
        ui_show_cash_process_result(STATUS_ERROR_NOT_FOUND);
        return;
    }

    if (request.type == CASH_TXN_DEPOSIT) {
        result = account_db_apply_deposit(target, request.amount);
    } else {
        result = account_db_apply_withdrawal(target, request.amount);
    }

    ui_show_cash_process_result(result);
}

/* ------------------------------------------------------------------- */
/* Top-level flows                                                       */
/* ------------------------------------------------------------------- */

static void run_bank_menu(AccountDatabase *account_db, BillQueue *bill_queue,
                          CashQueue *cash_queue) {
    int choice;
    bool keep_looping = true;

    do {
        SystemTime now;
        platform_get_current_time(&now);
        platform_clear_screen();
        ui_show_bank_menu_header(&now);

        if (!ui_collect_menu_choice(&choice)) {
            if (ui_is_input_exhausted()) {
                keep_looping = false;
                continue;
            }
            ui_show_invalid_option_message(&now);
            continue;
        }

        switch (choice) {
            case BANK_MENU_PAY_BILLS:
                handle_pay_bills(bill_queue);
                break;
            case BANK_MENU_CASH_QUEUE:
                handle_cash_queue_submit(cash_queue);
                break;
            case BANK_MENU_OPEN_ACCOUNT:
                handle_open_account(account_db);
                break;
            case BANK_MENU_VIEW_ACCOUNT:
                handle_view_account(account_db);
                break;
            case BANK_MENU_EXIT:
                keep_looping = false;
                break;
            default:
                ui_show_invalid_option_message(&now);
                break;
        }
    } while (keep_looping);
}

static void run_atm_session(AccountNode *acc) {
    int choice;
    bool keep_looping = true;

    do {
        SystemTime now;
        platform_get_current_time(&now);
        platform_clear_screen();
        ui_show_atm_menu_header(&now);

        if (!ui_collect_menu_choice(&choice)) {
            if (ui_is_input_exhausted()) {
                keep_looping = false;
                continue;
            }
            ui_show_invalid_option_message(&now);
            continue;
        }

        switch (choice) {
            case ATM_MENU_WITHDRAW:
                handle_atm_withdraw(acc);
                break;
            case ATM_MENU_BALANCE:
                handle_atm_balance(acc);
                break;
            case ATM_MENU_CHANGE_PIN:
                handle_atm_change_pin(acc);
                break;
            case ATM_MENU_EXIT:
                keep_looping = false;
                break;
            default:
                ui_show_invalid_option_message(&now);
                break;
        }
    } while (keep_looping);
}

static void run_atm_flow(AccountDatabase *account_db) {
    long account_number;
    int pin;
    SystemTime now;

    platform_get_current_time(&now);
    platform_clear_screen();
    ui_show_atm_login_header(&now);

    if (!ui_collect_atm_login(&account_number, &pin)) {
        ui_show_invalid_credentials_message(&now);
        return;
    }

    if (!account_db_check_atm_credentials(account_db, account_number, pin)) {
        ui_show_invalid_credentials_message(&now);
        return;
    }

    /* Looked up once at login; reused for the whole ATM session so the
       user is never re-asked for username/password mid-session (fixes
       an inconsistency in the original prototype -- see account.h,
       account_db_find_by_atm_credentials()). */
    AccountNode *acc = account_db_find_by_atm_credentials(account_db, account_number, pin);
    if (acc != NULL) {
        run_atm_session(acc);
    }
}

static void run_admin_account_openings_menu(AccountDatabase *account_db) {
    int choice;
    bool keep_looping = true;

    do {
        SystemTime now;
        platform_get_current_time(&now);
        platform_clear_screen();
        ui_show_admin_account_openings_header(&now);

        if (!ui_collect_menu_choice(&choice)) {
            if (ui_is_input_exhausted()) {
                keep_looping = false;
                continue;
            }
            ui_show_invalid_option_message(&now);
            continue;
        }

        switch (choice) {
            case ACC_OPEN_MENU_VIEW_PENDING:
                handle_view_pending_accounts(account_db);
                break;
            case ACC_OPEN_MENU_PROCESS:
                handle_process_pending_accounts(account_db);
                break;
            case ACC_OPEN_MENU_VIEW_ALL:
                handle_view_all_accounts(account_db);
                break;
            case ACC_OPEN_MENU_DELETE:
                handle_delete_account(account_db);
                break;
            case ACC_OPEN_MENU_EXIT:
                keep_looping = false;
                break;
            default:
                ui_show_invalid_option_message(&now);
                break;
        }
    } while (keep_looping);
}

static void run_admin_bill_payments_menu(BillQueue *bill_queue) {
    int choice;
    bool keep_looping = true;

    do {
        SystemTime now;
        platform_get_current_time(&now);
        platform_clear_screen();
        ui_show_admin_bill_payments_header(&now);

        if (!ui_collect_menu_choice(&choice)) {
            if (ui_is_input_exhausted()) {
                keep_looping = false;
                continue;
            }
            ui_show_invalid_option_message(&now);
            continue;
        }

        switch (choice) {
            case BILL_ADMIN_MENU_VIEW_ALL:
                handle_view_bill_queue(bill_queue);
                break;
            case BILL_ADMIN_MENU_VIEW_FRONT:
                handle_view_bill_front(bill_queue);
                break;
            case BILL_ADMIN_MENU_PROCESS_FRONT:
                handle_process_bill_front(bill_queue);
                break;
            case BILL_ADMIN_MENU_EXIT:
                keep_looping = false;
                break;
            default:
                ui_show_invalid_option_message(&now);
                break;
        }
    } while (keep_looping);
}

/** Admin cash-queue processing menu, mirroring the bill-payments menu
    (see note at handle_view_cash_queue() on why this exists -- SRS-029
    and SRS-030 complete the withdraw/deposit queue processing that the
    original prototype left unreachable). */
static void run_admin_cash_queue_menu(CashQueue *cash_queue, AccountDatabase *account_db) {
    int choice;
    bool keep_looping = true;

    do {
        SystemTime now;
        platform_get_current_time(&now);
        platform_clear_screen();
        ui_show_admin_cash_queue_header(&now);

        if (!ui_collect_menu_choice(&choice)) {
            if (ui_is_input_exhausted()) {
                keep_looping = false;
                continue;
            }
            ui_show_invalid_option_message(&now);
            continue;
        }

        switch (choice) {
            case BILL_ADMIN_MENU_VIEW_ALL:
                handle_view_cash_queue(cash_queue);
                break;
            case BILL_ADMIN_MENU_VIEW_FRONT:
                handle_view_cash_front(cash_queue);
                break;
            case BILL_ADMIN_MENU_PROCESS_FRONT:
                handle_process_cash_front(cash_queue, account_db);
                break;
            case BILL_ADMIN_MENU_EXIT:
                keep_looping = false;
                break;
            default:
                ui_show_invalid_option_message(&now);
                break;
        }
    } while (keep_looping);
}

static void run_admin_menu(AccountDatabase *account_db, BillQueue *bill_queue,
                           CashQueue *cash_queue) {
    int choice;
    bool keep_looping = true;

    do {
        SystemTime now;
        platform_get_current_time(&now);
        platform_clear_screen();
        ui_show_admin_menu_header(&now);

        if (!ui_collect_menu_choice(&choice)) {
            if (ui_is_input_exhausted()) {
                keep_looping = false;
                continue;
            }
            ui_show_invalid_option_message(&now);
            continue;
        }

        switch (choice) {
            case ADMIN_MENU_ACCOUNT_OPENINGS:
                run_admin_account_openings_menu(account_db);
                break;
            case ADMIN_MENU_BILL_PAYMENTS:
                run_admin_bill_payments_menu(bill_queue);
                break;
            case ADMIN_MENU_CASH_TRANSACTIONS:
                run_admin_cash_queue_menu(cash_queue, account_db);
                break;
            case ADMIN_MENU_EXIT:
                keep_looping = false;
                break;
            default:
                ui_show_invalid_option_message(&now);
                break;
        }
    } while (keep_looping);
}

static void run_admin_login_flow(AccountDatabase *account_db, BillQueue *bill_queue,
                                 CashQueue *cash_queue) {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    SystemTime now;

    platform_get_current_time(&now);
    platform_clear_screen();
    ui_show_admin_login_header(&now);

    if (!ui_collect_login_credentials(username, sizeof(username), password, sizeof(password))) {
        ui_show_invalid_credentials_message(&now);
        return;
    }

    if (!admin_check_credentials(username, password)) {
        ui_show_invalid_credentials_message(&now);
        return;
    }

    run_admin_menu(account_db, bill_queue, cash_queue);
}

int main(void) {
    AccountDatabase account_db;
    BillQueue bill_queue;
    CashQueue cash_queue;
    int choice;
    bool keep_looping = true;
    SystemTime now;

    account_db_init(&account_db);
    bill_queue_init(&bill_queue);
    cash_queue_init(&cash_queue);

    platform_get_current_time(&now);
    ui_show_start_screen(&now);
    platform_clear_screen();

    do {
        platform_get_current_time(&now);
        ui_show_main_menu_header(&now);

        if (!ui_collect_menu_choice(&choice)) {
            if (ui_is_input_exhausted()) {
                keep_looping = false;
                continue;
            }
            ui_show_invalid_option_message(&now);
            continue;
        }

        switch (choice) {
            case MAIN_MENU_ENTER_BANK:
                run_bank_menu(&account_db, &bill_queue, &cash_queue);
                break;
            case MAIN_MENU_ENTER_ATM:
                run_atm_flow(&account_db);
                break;
            case MAIN_MENU_ADMIN_LOGIN:
                run_admin_login_flow(&account_db, &bill_queue, &cash_queue);
                break;
            case MAIN_MENU_EXIT:
                platform_clear_screen();
                platform_get_current_time(&now);
                ui_show_end_screen(&now);
                keep_looping = false;
                break;
            default:
                ui_show_invalid_option_message(&now);
                break;
        }
    } while (keep_looping);

    account_db_destroy(&account_db);
    bill_queue_destroy(&bill_queue);
    cash_queue_destroy(&cash_queue);

    return 0;
}
