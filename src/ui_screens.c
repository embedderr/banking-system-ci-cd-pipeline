/**
 * @file ui_screens.c
 * @brief Implementation of all console input and output for the Banking
 *        System.
 *
 * This file is organized in two parts:
 *   1. Display functions (this pass) -- pure output, no return value.
 *   2. Collection functions (added next) -- read input into structs.
 */

#include "ui_screens.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===================================================================== */
/* Internal helpers                                                       */
/* ===================================================================== */

static void print_date_time_header(const SystemTime *time) {
    if (time == NULL) {
        return;
    }

    printf("\n\n\n\t\t\t Date: %d/%d/%d\n", time->day, time->month, time->year);
    printf("\t\t\t Time: %d:%02d\n", time->hour, time->minute);
}

static void print_bank_banner(void) {
    printf(
        "\t\t||===============================================================================||"
        "\n");
    printf(
        "\t\t                           APNA BANK                                               "
        "\n");
    printf(
        "\t\t                                                                                   "
        "\n");
    printf(
        "\t\t  When you see #, it means you should enter a number, such as 1, 2, 3, 4, 5, etc.  "
        "\n");
    printf(
        "\t\t___________________________________________________________________________________"
        "\n\n");
}

/* ===================================================================== */
/* Display Functions                                                      */
/* ===================================================================== */

void ui_show_start_screen(const SystemTime *time) {
    print_date_time_header(time);
    printf("\t\t\t\t\t\t\t||========================================||\n");
    printf("\t\t\t\t\t\t\t||           Welcome to APNA BANK          ||\n");
    printf("\t\t\t\t\t\t\t||========================================||\n\n");
    platform_wait_for_keypress();
}

void ui_show_end_screen(const SystemTime *time) {
    print_date_time_header(time);
    printf("\t\t\t\t\t\t\t||========================================||\n");
    printf("\t\t\t\t\t\t\t||     Thank You for using APNA BANK       ||\n");
    printf("\t\t\t\t\t\t\t||========================================||\n\n");
}

void ui_show_main_menu_header(const SystemTime *time) {
    print_date_time_header(time);
    printf("\t\t\t\t\t\t\t||========================================||\n");
    printf("\t\t\t\t\t\t\t||                APNA BANK                ||\n");
    printf("\t\t\t\t\t\t\t||      Choose an option:                  ||\n");
    printf("\t\t\t\t\t\t\t||          1. Enter Bank                  ||\n");
    printf("\t\t\t\t\t\t\t||          2. Enter ATM                   ||\n");
    printf("\t\t\t\t\t\t\t||          3. Admin Login                 ||\n");
    printf("\t\t\t\t\t\t\t||          4. Exit                        ||\n");
    printf("\t\t\t\t\t\t\t||========================================||\n\n");
}

void ui_show_bank_menu_header(const SystemTime *time) {
    print_date_time_header(time);
    printf("\t\t\t\t\t\t\t||===================================================||\n");
    printf("\t\t\t\t\t\t\t||             Welcome to APNA BANK                  ||\n");
    printf("\t\t\t\t\t\t\t||      Choose an option:                            ||\n");
    printf("\t\t\t\t\t\t\t||             1. Pay Bills                          ||\n");
    printf("\t\t\t\t\t\t\t||             2. Deposits/Withdrawals               ||\n");
    printf("\t\t\t\t\t\t\t||             3. Open up Account                    ||\n");
    printf("\t\t\t\t\t\t\t||             4. View your Account                  ||\n");
    printf("\t\t\t\t\t\t\t||             5. Exit                               ||\n");
    printf("\t\t\t\t\t\t\t||===================================================||\n\n");
}

void ui_show_atm_login_header(const SystemTime *time) {
    print_date_time_header(time);
    printf("\t\t\t\t\t\t\t||===================================================||\n");
    printf("\t\t\t\t\t\t\t||              Welcome to ATM                       ||\n");
    printf("\t\t\t\t\t\t\t||      Enter Following Details :=                   ||\n");
    printf("\t\t\t\t\t\t\t||             1. Account Number                     ||\n");
    printf("\t\t\t\t\t\t\t||             2. Debit Card PIN                     ||\n");
    printf("\t\t\t\t\t\t\t||===================================================||\n\n");
}

void ui_show_atm_menu_header(const SystemTime *time) {
    print_date_time_header(time);
    printf("\t\t\t\t\t\t\t||===================================================||\n");
    printf("\t\t\t\t\t\t\t||              Welcome to ATM                       ||\n");
    printf("\t\t\t\t\t\t\t||      Choose an option :=                          ||\n");
    printf("\t\t\t\t\t\t\t||             1. Withdraw Cash                      ||\n");
    printf("\t\t\t\t\t\t\t||             2. Balance Inquiry                    ||\n");
    printf("\t\t\t\t\t\t\t||             3. Change Pin                        ||\n");
    printf("\t\t\t\t\t\t\t||             4. Exit                               ||\n");
    printf("\t\t\t\t\t\t\t||===================================================||\n\n");
}

void ui_show_admin_login_header(const SystemTime *time) {
    print_date_time_header(time);
    printf("\t\t\t\t\t\t\t||===================================================||\n");
    printf("\t\t\t\t\t\t\t||              Welcome to Admin                     ||\n");
    printf("\t\t\t\t\t\t\t||      Enter Following Details :=                   ||\n");
    printf("\t\t\t\t\t\t\t||             1. Username                           ||\n");
    printf("\t\t\t\t\t\t\t||             2. Password                           ||\n");
    printf("\t\t\t\t\t\t\t||===================================================||\n\n");
}

void ui_show_admin_menu_header(const SystemTime *time) {
    print_date_time_header(time);
    printf("\t\t\t\t\t\t\t||===================================================||\n");
    printf("\t\t\t\t\t\t\t||              Welcome to Admin                     ||\n");
    printf("\t\t\t\t\t\t\t||      Choose an option :=                          ||\n");
    printf("\t\t\t\t\t\t\t||             1. Process Account Openings           ||\n");
    printf("\t\t\t\t\t\t\t||             2. Process Bill Payments              ||\n");
    printf("\t\t\t\t\t\t\t||             3. Process Cash Transactions          ||\n");
    printf("\t\t\t\t\t\t\t||             4. Exit                               ||\n");
    printf("\t\t\t\t\t\t\t||===================================================||\n\n");
}

void ui_show_admin_account_openings_header(const SystemTime *time) {
    print_date_time_header(time);
    printf("\t\t\t\t\t\t\t||===================================================||\n");
    printf("\t\t\t\t\t\t\t||          Welcome to Account Openings              ||\n");
    printf("\t\t\t\t\t\t\t||      Choose an option :=                          ||\n");
    printf("\t\t\t\t\t\t\t||             1. View Account Openings              ||\n");
    printf("\t\t\t\t\t\t\t||             2. Process Accounts                   ||\n");
    printf("\t\t\t\t\t\t\t||             3. View Accounts in Database          ||\n");
    printf("\t\t\t\t\t\t\t||             4. Delete an Account                  ||\n");
    printf("\t\t\t\t\t\t\t||             5. Exit                               ||\n");
    printf("\t\t\t\t\t\t\t||===================================================||\n\n");
}

void ui_show_admin_bill_payments_header(const SystemTime *time) {
    print_date_time_header(time);
    printf("\t\t\t\t\t\t\t||===================================================||\n");
    printf("\t\t\t\t\t\t\t||           Welcome to Bill Payments                ||\n");
    printf("\t\t\t\t\t\t\t||      Choose an option :=                          ||\n");
    printf("\t\t\t\t\t\t\t||             1. View Bill Payments                 ||\n");
    printf("\t\t\t\t\t\t\t||             2. View Bill Payment at Front         ||\n");
    printf("\t\t\t\t\t\t\t||             3. Process Bill Payment at Front      ||\n");
    printf("\t\t\t\t\t\t\t||             4. Exit                               ||\n");
    printf("\t\t\t\t\t\t\t||===================================================||\n\n");
}

void ui_show_admin_cash_queue_header(const SystemTime *time) {
    print_date_time_header(time);
    printf("\t\t\t\t\t\t\t||===================================================||\n");
    printf("\t\t\t\t\t\t\t||         Welcome to Cash Transactions              ||\n");
    printf("\t\t\t\t\t\t\t||      Choose an option :=                          ||\n");
    printf("\t\t\t\t\t\t\t||             1. View Cash Transaction Queue        ||\n");
    printf("\t\t\t\t\t\t\t||             2. View Cash Request at Front         ||\n");
    printf("\t\t\t\t\t\t\t||             3. Process Cash Request at Front      ||\n");
    printf("\t\t\t\t\t\t\t||             4. Exit                               ||\n");
    printf("\t\t\t\t\t\t\t||===================================================||\n\n");
}

void ui_show_invalid_option_message(const SystemTime *time) {
    print_date_time_header(time);
    print_bank_banner();
    printf("\t\t\t\t Wrong Option Entered, press any key to go back to Menu\n");
    platform_wait_for_keypress();
}

void ui_show_invalid_credentials_message(const SystemTime *time) {
    print_date_time_header(time);
    print_bank_banner();
    printf("\t\t\t\t # Invalid Credentials Entered\n");
    printf("\t\t\t\t Press any key to return to Menu\n");
    platform_wait_for_keypress();
}

/* ===================================================================== */
/* Result / Feedback Display Functions                                    */
/* ===================================================================== */

void ui_show_bill_enqueue_result(StatusCode result) {
    switch (result) {
        case STATUS_OK:
            printf("\t\t\t\t Transaction Completed, Bill payment in process.\n");
            break;
        case STATUS_ERROR_ALLOC_FAILED:
            printf("\t\t\t\t System error: could not allocate memory for this request.\n");
            break;
        default:
            printf("\t\t\t\t ERROR! Invalid input, transaction failed.\n");
            break;
    }
    printf("\t\t\t\t Press any key to return to Bank Menu\n");
    platform_wait_for_keypress();
}

void ui_show_bill_process_result(StatusCode result) {
    switch (result) {
        case STATUS_OK:
            printf("\t\t\t\t Bill payment at front of queue has been processed.\n");
            break;
        case STATUS_ERROR_QUEUE_EMPTY:
            printf("\t\t\t\t Bill Payment Queue is empty, nothing left to process.\n");
            break;
        default:
            printf("\t\t\t\t ERROR! Could not process this bill payment.\n");
            break;
    }
    printf("\t\t\t\t Press any key to return to Bank Menu\n");
    platform_wait_for_keypress();
}

void ui_show_cash_enqueue_result(StatusCode result) {
    switch (result) {
        case STATUS_OK:
            printf("\t\t\t\t Transaction Completed.\n");
            break;
        case STATUS_ERROR_ALLOC_FAILED:
            printf("\t\t\t\t System error: could not allocate memory for this request.\n");
            break;
        default:
            printf("\t\t\t\t ERROR! Invalid input, transaction failed.\n");
            break;
    }
    printf("\t\t\t\t Press any key to return to Bank Menu\n");
    platform_wait_for_keypress();
}

void ui_show_cash_process_result(StatusCode result) {
    switch (result) {
        case STATUS_OK:
            printf("\t\t\t\t Cash request at front of queue has been processed.\n");
            break;
        case STATUS_ERROR_QUEUE_EMPTY:
            printf("\t\t\t\t Cash Transaction Queue is empty, nothing left to process.\n");
            break;
        case STATUS_ERROR_NOT_FOUND:
            printf("\t\t\t\t Target account no longer exists; request could not be applied.\n");
            break;
        case STATUS_ERROR_INSUFFICIENT_FUNDS:
            printf("\t\t\t\t Target account has insufficient funds; withdrawal not applied.\n");
            break;
        default:
            printf("\t\t\t\t ERROR! Could not process this cash request.\n");
            break;
    }
    printf("\t\t\t\t Press any key to return to Bank Menu\n");
    platform_wait_for_keypress();
}

void ui_show_account_request_result(StatusCode result) {
    switch (result) {
        case STATUS_OK:
            printf("\t\t\t\t Account Details Completed, in process now.\n");
            break;
        case STATUS_ERROR_DUPLICATE:
            printf("\t\t\t\t Username has already been taken.\n");
            break;
        case STATUS_ERROR_ALLOC_FAILED:
            printf("\t\t\t\t System error: could not allocate memory for this request.\n");
            break;
        default:
            printf("\t\t\t\t ERROR! Invalid input, account opening failed.\n");
            break;
    }
    printf("\t\t\t\t Press any key to return to Bank Menu\n");
    platform_wait_for_keypress();
}

void ui_show_account_decision_result(StatusCode result, bool approved) {
    if (result == STATUS_OK) {
        if (approved) {
            printf("\t\t\t\t Account approved and a new account number has been assigned.\n");
        } else {
            printf("\t\t\t\t Account declined.\n");
        }
    } else {
        printf("\t\t\t\t ERROR! Could not process this account.\n");
    }
}

void ui_show_account_delete_result(StatusCode result) {
    switch (result) {
        case STATUS_OK:
            printf("\t\t\t\t Account found in database and successfully deleted.\n");
            break;
        case STATUS_ERROR_NOT_FOUND:
            printf("\t\t\t\t Account not found in database.\n");
            break;
        default:
            printf("\t\t\t\t ERROR! Invalid input, deletion failed.\n");
            break;
    }
    printf("\t\t\t\t Press any key to return to Bank Menu\n");
    platform_wait_for_keypress();
}

void ui_show_withdrawal_result(StatusCode result, Money remaining_balance) {
    switch (result) {
        case STATUS_OK:
            printf("\t\t\t\tTransaction successful!\n");
            printf("\t\t\t\tRemaining Balance -> %.2f\n", remaining_balance);
            break;
        case STATUS_ERROR_INSUFFICIENT_FUNDS:
            printf("\t\t\t\tAccount does not contain this much cash, transaction failed.\n");
            break;
        case STATUS_ERROR_NOT_FOUND:
            printf("\t\t\t\tAccount not found in database, transaction failed.\n");
            break;
        default:
            printf("\t\t\t\tERROR! Invalid input, transaction failed.\n");
            break;
    }
    printf("\t\t\t\tPress any key to return to Bank Menu\n");
    platform_wait_for_keypress();
}

void ui_show_pin_change_result(StatusCode result) {
    switch (result) {
        case STATUS_OK:
            printf("\t\t\t\tProcess successful! PIN Updated.\n");
            break;
        case STATUS_ERROR_NOT_FOUND:
            printf("\t\t\t\tAccount not found in database, transaction failed.\n");
            break;
        default:
            printf("\t\t\t\tNew PIN is not valid or Old PIN is incorrect, process failed.\n");
            break;
    }
    printf("\t\t\t\tPress any key to return to Bank Menu\n");
    platform_wait_for_keypress();
}

void ui_show_balance(long account_number, Money balance) {
    printf("\t\t\t\tAccount# -> %ld\n", account_number);
    printf("\t\t\t\tBalance -> %.2f\n", balance);
    printf("\t\t\t\tPress any key to return to Bank Menu\n");
    platform_wait_for_keypress();
}

void ui_show_account_details(const AccountNode *acc) {
    const char *status_text;

    if (acc == NULL) {
        printf("\t\t\t\tAccount not found in database.\n");
        return;
    }

    switch (acc->status) {
        case ACCOUNT_STATUS_PENDING:
            status_text = "Pending";
            break;
        case ACCOUNT_STATUS_COMPLETED:
            status_text = "Completed";
            break;
        case ACCOUNT_STATUS_DECLINED:
            status_text = "Declined";
            break;
        default:
            status_text = "Unknown";
            break;
    }

    printf("\t\t\t\t------------------------------\n");
    printf("\t\t\t\tAccount#: %ld\n", acc->account_number);
    printf("\t\t\t\tAccount Status: %s\n", status_text);
    printf("\t\t\t\tFirst Name: %s\n", acc->details.first_name);
    printf("\t\t\t\tLast Name: %s\n", acc->details.last_name);
    printf("\t\t\t\tEmail Address: %s\n", acc->details.email);
    printf("\t\t\t\tPhone#: %lld\n", acc->details.phone_number);
    printf("\t\t\t\tBalance: %.2f\n", acc->balance);
    printf("\t\t\t\t ---Address--- \n");
    address_display(&acc->details.home_address);
    printf("\t\t\t\t -------------\n");
    printf("\t\t\t\tUsername: %s\n", acc->details.username);
    printf("\t\t\t\tDebit Card: %s\n", acc->details.wants_debit_card ? "Yes" : "No");
    printf("\t\t\t\tCheque Book: %s\n", acc->details.wants_cheque_book ? "Yes" : "No");
    printf("\t\t\t\t------------------------------\n\n");
}

void ui_show_bill_request(const BillRequestInput *req, int index) {
    if (req == NULL) {
        return;
    }

    printf("\t\t\t\t--------------- #%d ---------------\n", index);
    printf("\t\t\t\tBill Type: %s\n", req->bill_type);
    printf("\t\t\t\tBill Invoice#: %ld\n", req->invoice_number);
    printf("\t\t\t\tBill Amount: %.2f\n", req->amount);
    printf("\t\t\t\t-----------------------------------\n\n");
}

void ui_show_cash_request(const CashRequestInput *req, int index) {
    if (req == NULL) {
        return;
    }

    printf("\t\t\t\t--------------- #%d ---------------\n", index);
    printf("\t\t\t\tAccount#: %ld\n", req->account_number);
    printf("\t\t\t\tAmount: %.2f\n", req->amount);
    printf("\t\t\t\tType: %s\n", (req->type == CASH_TXN_DEPOSIT) ? "Deposit" : "Withdraw");
    printf("\t\t\t\t-----------------------------------\n\n");
}

void ui_show_queue_empty_message(const char *queue_label) {
    printf("\t\t\t\t %s Queue is empty\n", (queue_label != NULL) ? queue_label : "The");
    printf("\t\t\t\t Press any key to return to Bank Menu\n");
    platform_wait_for_keypress();
}

void ui_show_database_empty_message(void) {
    printf("\t\t\t\t Account Database is empty\n");
    printf("\t\t\t\t Press any key to return to Bank Menu\n");
    platform_wait_for_keypress();
}

/* ===================================================================== */
/* Internal input-parsing helpers                                        */
/* ===================================================================== */

#define UI_LINE_BUFFER_SIZE 128U

/**
 * @brief Reads one line from stdin into @p buffer, stripping the
 *        trailing newline if present.
 *
 * @return true if a line was successfully read; false on EOF or error.
 */
static bool read_line(char *buffer, size_t size) {
    size_t len;

    if (fgets(buffer, (int)size, stdin) == NULL) {
        return false;
    }

    len = strlen(buffer);
    if ((len > 0U) && (buffer[len - 1U] == '\n')) {
        buffer[len - 1U] = '\0';
    }

    return true;
}

/**
 * @brief Parses @p str as a base-10 long integer, rejecting any input
 *        that is empty or contains trailing non-numeric characters
 *        (SRS-027: reject invalid numeric input rather than proceeding
 *        with an indeterminate value).
 */
static bool parse_long(const char *str, long *out) {
    char *end_ptr;
    long value;

    if ((str == NULL) || (str[0] == '\0')) {
        return false;
    }

    value = strtol(str, &end_ptr, 10);
    if (*end_ptr != '\0') {
        return false;
    }

    *out = value;
    return true;
}

static bool parse_int(const char *str, int *out) {
    long value;

    if (!parse_long(str, &value)) {
        return false;
    }

    *out = (int)value;
    return true;
}

static bool parse_long_long(const char *str, long long *out) {
    char *end_ptr;
    long long value;

    if ((str == NULL) || (str[0] == '\0')) {
        return false;
    }

    value = strtoll(str, &end_ptr, 10);
    if (*end_ptr != '\0') {
        return false;
    }

    *out = value;
    return true;
}

static bool parse_double(const char *str, double *out) {
    char *end_ptr;
    double value;

    if ((str == NULL) || (str[0] == '\0')) {
        return false;
    }

    value = strtod(str, &end_ptr);
    if (*end_ptr != '\0') {
        return false;
    }

    *out = value;
    return true;
}

/**
 * @brief Copies @p src into @p dest if it fits (including the null
 *        terminator); rejects (returns false) rather than truncating.
 */
static bool copy_bounded(char *dest, size_t dest_size, const char *src) {
    size_t src_len = strlen(src);

    if (src_len >= dest_size) {
        return false;
    }

    strcpy(dest, src);
    return true;
}

/* ===================================================================== */
/* Collection Functions                                                   */
/* ===================================================================== */

bool ui_collect_menu_choice(int *out_choice) {
    char line[UI_LINE_BUFFER_SIZE];

    printf("\t\t\t\t\t\tEnter Option: ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }

    return parse_int(line, out_choice);
}

bool ui_confirm_yes_no(const char *prompt_message) {
    char line[UI_LINE_BUFFER_SIZE];

    if (prompt_message != NULL) {
        printf("\t\t\t\t %s (Y/N): ", prompt_message);
    }

    if (!read_line(line, sizeof(line))) {
        return false;
    }

    return (line[0] == 'y') || (line[0] == 'Y');
}

bool ui_collect_bill_request(BillRequestInput *out_input) {
    char line[UI_LINE_BUFFER_SIZE];
    long invoice_number;
    double amount;

    if (out_input == NULL) {
        return false;
    }

    printf("\t\t\t\t Bill Type (Electricity, Gas, Internet) -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!copy_bounded(out_input->bill_type, sizeof(out_input->bill_type), line)) {
        return false;
    }

    printf("\t\t\t\t Invoice Number -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_long(line, &invoice_number)) {
        return false;
    }
    out_input->invoice_number = invoice_number;

    printf("\t\t\t\t Bill Amount -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_double(line, &amount)) {
        return false;
    }
    out_input->amount = amount;

    return true;
}

bool ui_collect_cash_request(CashRequestInput *out_input) {
    char line[UI_LINE_BUFFER_SIZE];
    int type_choice;
    long account_number;
    double amount;

    if (out_input == NULL) {
        return false;
    }

    printf("\t\t\t\t 1) Withdraw Cash  2) Deposit Cash -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_int(line, &type_choice)) {
        return false;
    }
    if (type_choice == 1) {
        out_input->type = CASH_TXN_WITHDRAW;
    } else if (type_choice == 2) {
        out_input->type = CASH_TXN_DEPOSIT;
    } else {
        return false;
    }

    printf("\t\t\t\t Account Number -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_long(line, &account_number)) {
        return false;
    }
    out_input->account_number = account_number;

    printf("\t\t\t\t Cash Amount -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_double(line, &amount)) {
        return false;
    }
    out_input->amount = amount;

    return true;
}

bool ui_collect_new_account_request(NewAccountInput *out_input) {
    char line[UI_LINE_BUFFER_SIZE];
    long temp_long;
    long long temp_ll;
    int debit_choice;
    int cheque_choice;

    if (out_input == NULL) {
        return false;
    }

    address_init(&out_input->home_address);

    printf("\t\t\t\t First Name -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!copy_bounded(out_input->first_name, sizeof(out_input->first_name), line)) {
        return false;
    }

    printf("\t\t\t\t Last Name -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!copy_bounded(out_input->last_name, sizeof(out_input->last_name), line)) {
        return false;
    }

    printf("\t\t\t\t Email Address -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!copy_bounded(out_input->email, sizeof(out_input->email), line)) {
        return false;
    }

    printf("\t\t\t\t House Number -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_long(line, &temp_long)) {
        return false;
    }
    out_input->home_address.house_number = (int)temp_long;

    printf("\t\t\t\t Town Name -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!copy_bounded(out_input->home_address.town_name, sizeof(out_input->home_address.town_name),
                      line)) {
        return false;
    }

    printf("\t\t\t\t Street Number -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_long(line, &temp_long)) {
        return false;
    }
    out_input->home_address.street_number = (int)temp_long;

    printf("\t\t\t\t City -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!copy_bounded(out_input->home_address.city, sizeof(out_input->home_address.city), line)) {
        return false;
    }

    printf("\t\t\t\t Phone Number -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_long_long(line, &temp_ll)) {
        return false;
    }
    out_input->phone_number = temp_ll;

    printf("\t\t\t\t Aadhaar Number -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_long_long(line, &temp_ll)) {
        return false;
    }
    out_input->aadhaar_number = temp_ll;

    printf("\t\t\t\t Debit Card? 1=Yes 0=No -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_int(line, &debit_choice)) {
        return false;
    }

    if (debit_choice == 1) {
        int pin;
        out_input->wants_debit_card = true;
        printf("\t\t\t\t Debit Card PIN (4 digits) -> ");
        if (!read_line(line, sizeof(line))) {
            return false;
        }
        if (!parse_int(line, &pin)) {
            return false;
        }
        out_input->debit_card_pin = pin;
    } else {
        out_input->wants_debit_card = false;
        out_input->debit_card_pin = 0;
    }

    printf("\t\t\t\t Cheque Book? 1=Yes 0=No -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_int(line, &cheque_choice)) {
        return false;
    }
    out_input->wants_cheque_book = (cheque_choice == 1);

    printf("\t\t\t\t Desired Username (no spaces) -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!copy_bounded(out_input->username, sizeof(out_input->username), line)) {
        return false;
    }

    printf("\t\t\t\t Desired Password (no spaces) -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!copy_bounded(out_input->password, sizeof(out_input->password), line)) {
        return false;
    }

    return true;
}

bool ui_collect_login_credentials(char *out_username, size_t username_cap, char *out_password,
                                  size_t password_cap) {
    char line[UI_LINE_BUFFER_SIZE];

    if ((out_username == NULL) || (out_password == NULL)) {
        return false;
    }

    printf("\t\t\t Enter Username: ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!copy_bounded(out_username, username_cap, line)) {
        return false;
    }

    printf("\t\t\t Enter Password: ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!copy_bounded(out_password, password_cap, line)) {
        return false;
    }

    return true;
}

bool ui_collect_atm_login(long *out_account_number, int *out_pin) {
    char line[UI_LINE_BUFFER_SIZE];
    long account_number;
    int pin;

    if ((out_account_number == NULL) || (out_pin == NULL)) {
        return false;
    }

    printf("\t\t\t\t Account Number -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_long(line, &account_number)) {
        return false;
    }

    printf("\t\t\t\t Debit Card PIN -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_int(line, &pin)) {
        return false;
    }

    *out_account_number = account_number;
    *out_pin = pin;
    return true;
}

bool ui_collect_pin_change(int *out_old_pin, int *out_new_pin) {
    char line[UI_LINE_BUFFER_SIZE];
    int old_pin;
    int new_pin;

    if ((out_old_pin == NULL) || (out_new_pin == NULL)) {
        return false;
    }

    printf("\t\t\t\t Old PIN -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_int(line, &old_pin)) {
        return false;
    }

    printf("\t\t\t\t New PIN (4 digits) -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_int(line, &new_pin)) {
        return false;
    }

    *out_old_pin = old_pin;
    *out_new_pin = new_pin;
    return true;
}

bool ui_collect_withdrawal_amount(Money *out_amount) {
    char line[UI_LINE_BUFFER_SIZE];
    double amount;

    if (out_amount == NULL) {
        return false;
    }

    printf("\t\t\t\t Amount to withdraw -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_double(line, &amount)) {
        return false;
    }

    *out_amount = amount;
    return true;
}

bool ui_collect_account_number(long *out_account_number) {
    char line[UI_LINE_BUFFER_SIZE];
    long account_number;

    if (out_account_number == NULL) {
        return false;
    }

    printf("\t\t\t\t Account Number -> ");
    if (!read_line(line, sizeof(line))) {
        return false;
    }
    if (!parse_long(line, &account_number)) {
        return false;
    }

    *out_account_number = account_number;
    return true;
}

bool ui_is_input_exhausted(void) { return feof(stdin) != 0; }
