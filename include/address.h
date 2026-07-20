/**
 * @file address.h
 * @brief Represents a customer's home address and provides display and
 *        accessor functions for it.
 *
 * Per the I/O Separation Principle (docs/design/LLD.md, Section 1.2),
 * this module does NOT collect input from the user — that responsibility
 * belongs to ui_screens, which fills an Address struct directly and
 * passes it onward. This module only initializes, displays, and reads
 * back fields of an already-populated Address.
 *
 * Traceability: supports SRS-001 (account opening captures home address).
 */

#ifndef ADDRESS_H
#define ADDRESS_H

#include "common.h"

/**
 * @brief A customer's home address.
 */
typedef struct {
    int house_number;             /**< House/building number. */
    char town_name[MAX_TOWN_LEN]; /**< Town name. */
    int street_number;            /**< Street number. */
    char city[MAX_CITY_LEN];      /**< City name. */
} Address;

/**
 * @brief Initializes @p addr to a well-defined empty state
 *        (all numeric fields 0, all strings empty).
 *
 * @param addr  Non-NULL pointer to the Address to initialize. If NULL,
 *              the function returns immediately without writing anything.
 */
void address_init(Address *addr);

/**
 * @brief Prints @p addr to the console in a human-readable format.
 *
 * @param addr  Non-NULL pointer to the Address to display. If NULL, the
 *              function prints nothing.
 */
void address_display(const Address *addr);

/**
 * @brief Returns the house number stored in @p addr.
 *
 * @param addr  Non-NULL pointer to the Address to read.
 * @return The house number, or 0 if @p addr is NULL.
 */
int address_get_house_number(const Address *addr);

/**
 * @brief Returns the street number stored in @p addr.
 *
 * @note The original C++ prototype's getStreetNumber() incorrectly
 *       returned houseNumber instead of streetNumber. This function
 *       fixes that bug.
 *
 * @param addr  Non-NULL pointer to the Address to read.
 * @return The street number, or 0 if @p addr is NULL.
 */
int address_get_street_number(const Address *addr);

/**
 * @brief Returns a pointer to the city name stored in @p addr.
 *
 * @param addr  Non-NULL pointer to the Address to read.
 * @return Pointer to the null-terminated city string, or an empty string
 *         literal if @p addr is NULL. The returned pointer is valid only
 *         as long as @p addr itself is valid.
 */
const char *address_get_city(const Address *addr);

/**
 * @brief Returns a pointer to the town name stored in @p addr.
 *
 * @param addr  Non-NULL pointer to the Address to read.
 * @return Pointer to the null-terminated town name string, or an empty
 *         string literal if @p addr is NULL. The returned pointer is
 *         valid only as long as @p addr itself is valid.
 */
const char *address_get_town_name(const Address *addr);

#endif /* ADDRESS_H */
