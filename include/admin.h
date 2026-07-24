/**
 * @file admin.h
 * @brief Administrator authentication.
 *
 * Traceability: satisfies SRS-009 (administrator must authenticate before
 * accessing any admin function).
 */

#ifndef ADMIN_H
#define ADMIN_H

#include <stdbool.h>

/**
 * @brief Checks whether @p username and @p password match the
 *        designated administrator credentials.
 *
 * The reference credentials are fixed constants defined internally in
 * admin.c and are not exposed via this header, matching the original
 * design intent of a single hardcoded administrator account.
 *
 * @param username  Non-NULL, null-terminated username to check.
 * @param password  Non-NULL, null-terminated password to check.
 * @return true if both values match the administrator credentials;
 *         false otherwise, or if either argument is NULL.
 */
bool admin_check_credentials(const char *username, const char *password);

#endif /* ADMIN_H */
