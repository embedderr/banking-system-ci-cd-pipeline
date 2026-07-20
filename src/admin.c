/**
 * @file admin.c
 * @brief Implementation of administrator authentication.
 *
 * Design note (known limitation, documented rather than hidden): the
 * administrator credentials are fixed, hardcoded constants, matching the
 * original prototype's design. A production system would instead store
 * a hashed credential (e.g. via a secure password-hashing function) and
 * load it from a protected configuration source, not compile it into the
 * binary in plaintext. This is out of scope for the current version but
 * is flagged here for future revision.
 */

#include "admin.h"

#include <string.h>

static const char ADMIN_USERNAME[] = "admin123";
static const char ADMIN_PASSWORD[] = "helloworld";

bool admin_check_credentials(const char *username, const char *password) {
    if ((username == NULL) || (password == NULL)) {
        return false;
    }

    return (strcmp(username, ADMIN_USERNAME) == 0) && (strcmp(password, ADMIN_PASSWORD) == 0);
}
