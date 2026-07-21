/**
 * @file ui_screens.h
 * @brief All console input and output for the Banking System.
 *
 * Per the I/O Separation Principle (docs/design/LLD.md, Section 1.2),
 * every scanf/printf call in the system lives in this module. Every
 * other module is pure logic and performs no I/O of its own.
 *
 * Two categories of functions:
 *   - Display functions: print a screen, return nothing.
 *   - Collection functions: read raw input into an Input struct, return
 *     true/false indicating whether the input was well-formed.
 *
 * Traceability: no direct SRS (presentation-layer support for all
 * functional SRS); enforces SRS-027 (reject invalid numeric input) at
 * the point of input collection.
 */

#ifndef UI_SCREENS_H
#define UI_SCREENS_H

#include <stdbool.h>
#include <stddef.h>
#include "common.h"
#include "platform.h"
#include "address.h"
#include "bill_queue.h"
#include "cash_queue.h"
#include "account.h"

/* ===================================================================== */
/* Display Functions                                                      */
/* ===================================================================== */

/** Prints the application's welcome screen and waits for a keypress. */
void ui_show_start_screen(const SystemTime *time);

/** Prints the application's closing screen. */
void ui_show_end_screen(const SystemTime *time);

/** Prints the top-level menu (Enter Bank / Enter ATM / Admin Login / Exit). */
void ui_show_main_menu_header(const SystemTime *time);

/** Prints the customer bank menu (Pay Bills / Deposits.../ Open Account / ...). */
void ui_show_bank_menu_header(const SystemTime *time);

/** Prints the ATM login prompt header (before account number/PIN are read). */
void ui_show_atm_login_header(const SystemTime *time);

/** Prints the ATM operations menu (Withdraw / Balance Enquiry / Change PIN / Exit). */
void ui_show_atm_menu_header(const SystemTime *time);

/** Prints the admin login prompt header (before username/password are read). */
void ui_show_admin_login_header(const SystemTime *time);

/** Prints the admin operations menu (Account Openings / Bill Payments / Exit). */
void ui_show_admin_menu_header(const SystemTime *time);

/** Prints the admin account-openings sub-menu. */
void ui_show_admin_account_openings_header(const SystemTime *time);

/** Prints the admin bill-payments sub-menu. */
void ui_show_admin_bill_payments_header(const SystemTime *time);

/** Prints the admin cash-transactions sub-menu. */
void ui_show_admin_cash_queue_header(const SystemTime *time);

/** Prints a generic "wrong option, returning to menu" message and waits for a keypress. */
void ui_show_invalid_option_message(const SystemTime *time);

/** Prints a generic "invalid credentials" message and waits for a keypress. */
void ui_show_invalid_credentials_message(const SystemTime *time);

/* ===================================================================== */
/* Result / Feedback Display Functions                                    */
/* ===================================================================== */

/** Displays the outcome of a bill payment enqueue attempt. */
void ui_show_bill_enqueue_result(StatusCode result);

/** Displays the outcome of processing (dequeuing) the front bill payment request. */
void ui_show_bill_process_result(StatusCode result);

/** Displays the outcome of a cash withdraw/deposit enqueue attempt. */
void ui_show_cash_enqueue_result(StatusCode result);

/** Displays the outcome of processing (dequeuing and applying) the front cash request. */
void ui_show_cash_process_result(StatusCode result);

/** Displays the outcome of a new account request submission. */
void ui_show_account_request_result(StatusCode result);

/** Displays the outcome of an account approval/decline action. */
void ui_show_account_decision_result(StatusCode result, bool approved);

/** Displays the outcome of an account deletion attempt. */
void ui_show_account_delete_result(StatusCode result);

/** Displays the outcome of a withdrawal attempt at the ATM. */
void ui_show_withdrawal_result(StatusCode result, Money remaining_balance);

/** Displays the outcome of a PIN change attempt. */
void ui_show_pin_change_result(StatusCode result);

/** Displays a balance enquiry result. */
void ui_show_balance(long account_number, Money balance);

/** Displays full details of a single account record (used for self-service view and admin views). */
void ui_show_account_details(const AccountNode *acc);

/** Displays a single bill payment request (used when showing the queue). */
void ui_show_bill_request(const BillRequestInput *req, int index);

/** Displays a single cash withdraw/deposit request (used when showing the queue). */
void ui_show_cash_request(const CashRequestInput *req, int index);

/** Displays a generic "queue is empty" message. */
void ui_show_queue_empty_message(const char *queue_label);

/** Displays a generic "database is empty" message. */
void ui_show_database_empty_message(void);

/* ===================================================================== */
/* Collection Functions                                                   */
/* ===================================================================== */

/**
 * @brief Reads a menu choice as an integer.
 *
 * @param out_choice  Non-NULL pointer to receive the parsed choice.
 * @return true if a valid integer was read; false if the input could
 *         not be parsed as a number (SRS-027).
 */
bool ui_collect_menu_choice(int *out_choice);

/**
 * @brief Reads a Y/N confirmation from the user.
 *
 * @param prompt_message  Message to display before reading input.
 * @return true if the user entered 'y' or 'Y'; false for any other input.
 */
bool ui_confirm_yes_no(const char *prompt_message);

/**
 * @brief Collects a complete bill payment request.
 *
 * @param out_input  Non-NULL pointer to receive the collected data.
 * @return true if all fields were read successfully; false otherwise.
 */
bool ui_collect_bill_request(BillRequestInput *out_input);

/**
 * @brief Collects a complete cash withdraw/deposit request.
 *
 * @param out_input  Non-NULL pointer to receive the collected data.
 * @return true if all fields were read successfully; false otherwise.
 */
bool ui_collect_cash_request(CashRequestInput *out_input);

/**
 * @brief Collects a complete new account request, including a nested
 *        Address collected via address-related prompts.
 *
 * @param out_input  Non-NULL pointer to receive the collected data.
 * @return true if all fields were read successfully; false otherwise.
 */
bool ui_collect_new_account_request(NewAccountInput *out_input);

/**
 * @brief Collects a username/password pair (used for self-service
 *        account viewing and admin login).
 *
 * @param out_username      Non-NULL buffer to receive the username.
 * @param username_cap      Capacity, in bytes, of @p out_username.
 * @param out_password      Non-NULL buffer to receive the password.
 * @param password_cap      Capacity, in bytes, of @p out_password.
 * @return true if both fields were read successfully; false otherwise.
 */
bool ui_collect_login_credentials(char *out_username, size_t username_cap,
                                   char *out_password, size_t password_cap);

/**
 * @brief Collects an account number and debit card PIN for ATM login.
 *
 * @param out_account_number  Non-NULL pointer to receive the account number.
 * @param out_pin             Non-NULL pointer to receive the PIN.
 * @return true if both fields were read successfully as numbers; false
 *         otherwise (SRS-027).
 */
bool ui_collect_atm_login(long *out_account_number, int *out_pin);

/**
 * @brief Collects an old PIN and new PIN for a PIN-change request.
 *
 * @param out_old_pin  Non-NULL pointer to receive the current PIN.
 * @param out_new_pin  Non-NULL pointer to receive the desired new PIN.
 * @return true if both fields were read successfully as numbers; false
 *         otherwise (SRS-027).
 */
bool ui_collect_pin_change(int *out_old_pin, int *out_new_pin);

/**
 * @brief Collects a withdrawal amount for an ATM withdrawal request.
 *
 * @param out_amount  Non-NULL pointer to receive the amount.
 * @return true if the amount was read successfully as a number; false
 *         otherwise (SRS-027).
 */
bool ui_collect_withdrawal_amount(Money *out_amount);

/**
 * @brief Collects an account number for an admin delete-account action.
 *
 * @param out_account_number  Non-NULL pointer to receive the account number.
 * @return true if the value was read successfully as a number; false
 *         otherwise (SRS-027).
 */
bool ui_collect_account_number(long *out_account_number);

/**
 * @brief Checks whether stdin has been permanently exhausted (EOF), as
 *        opposed to a single malformed line of input.
 *
 * Callers should treat this as a signal to exit gracefully rather than
 * looping and re-prompting -- a closed/empty stdin (e.g. a CI runner
 * invoking this program with no piped input) would otherwise cause an
 * infinite "invalid option, please retry" loop, since every subsequent
 * read also immediately fails.
 *
 * @return true if stdin has hit end-of-file; false otherwise.
 */
bool ui_is_input_exhausted(void);

#endif /* UI_SCREENS_H */
