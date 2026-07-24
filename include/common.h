/**
 * @file common.h
 * @brief Shared types, status codes, and buffer-size constants used across
 *        every module in the Banking System.
 *
 * This header has no corresponding .c file — it contains only definitions
 * that other modules depend on. It sits at the bottom of the module
 * dependency graph (see docs/design/HLD.md, Section 2) and must never
 * include any other project header.
 *
 * Traceability: supports SRS-026 (modular organization) as the shared
 * foundation every other module depends on.
 */

#ifndef COMMON_H
#define COMMON_H

#include <stddef.h> /* size_t */

/* ------------------------------------------------------------------- */
/* Money type                                                           */
/* ------------------------------------------------------------------- */

/**
 * @brief Represents a monetary amount.
 *
 * Design note (see docs/design/HLD.md, Section 1.3): a real production
 * banking system would typically store currency as integer minor units
 * (e.g. paise) to avoid floating-point rounding error. For this project's
 * scope (a learning / CI-pipeline demonstration, not a real financial
 * system), a floating type is retained for simplicity. This is a
 * deliberate, documented trade-off.
 */
typedef double Money;

/* ------------------------------------------------------------------- */
/* Status codes                                                         */
/* ------------------------------------------------------------------- */

/**
 * @brief Result code returned by every business-logic function that can
 *        fail. Callers must check this before trusting any output
 *        parameter the function may have written.
 */
typedef enum {
    STATUS_OK = 0,          /**< Operation completed successfully. */
    STATUS_ERROR_NOT_FOUND, /**< Requested record does not exist. */
    STATUS_ERROR_DUPLICATE, /**< A uniqueness constraint was violated (e.g. username taken). */
    STATUS_ERROR_INSUFFICIENT_FUNDS, /**< Withdrawal amount exceeds available balance. */
    STATUS_ERROR_INVALID_INPUT,      /**< Input failed validation (e.g. PIN not 4 digits). */
    STATUS_ERROR_ALLOC_FAILED,       /**< A memory allocation (malloc) failed. */
    STATUS_ERROR_QUEUE_EMPTY         /**< Attempted to dequeue/peek an empty queue. */
} StatusCode;

/* ------------------------------------------------------------------- */
/* Buffer size constants                                                */
/* ------------------------------------------------------------------- */

/** Maximum length (including null terminator) for a first or last name. */
#define MAX_NAME_LEN 50U

/** Maximum length (including null terminator) for a username. */
#define MAX_USERNAME_LEN 30U

/** Maximum length (including null terminator) for a password. */
#define MAX_PASSWORD_LEN 30U

/** Maximum length (including null terminator) for an email address. */
#define MAX_EMAIL_LEN 60U

/** Maximum length (including null terminator) for a city name. */
#define MAX_CITY_LEN 40U

/** Maximum length (including null terminator) for a town name. */
#define MAX_TOWN_LEN 40U

/** Maximum length (including null terminator) for a bill type string
 *  (e.g. "Electricity", "Gas", "Internet"). */
#define MAX_BILL_TYPE_LEN 20U

#endif /* COMMON_H */
