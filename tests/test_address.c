/**
 * @file test_address.c
 * @brief Unity unit tests for the address module.
 *
 * Implements test cases UT-001 through UT-011 as specified in
 * docs/testing/unit_test_cases.xlsx.
 */

#include "unity.h"
#include "address.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* UT-001: address_init on a fresh struct zeroes all fields. */
void test_UT_001_address_init_positive(void)
{
    Address addr;
    address_init(&addr);

    TEST_ASSERT_EQUAL_INT(0, addr.house_number);
    TEST_ASSERT_EQUAL_INT(0, addr.street_number);
    TEST_ASSERT_EQUAL_STRING("", addr.town_name);
    TEST_ASSERT_EQUAL_STRING("", addr.city);
}

/* UT-002: address_init with a NULL pointer must not crash. */
void test_UT_002_address_init_null_safety(void)
{
    address_init(NULL);
    TEST_PASS();
}

/* UT-003: address_get_house_number returns the correct value. */
void test_UT_003_get_house_number_positive(void)
{
    Address addr;
    address_init(&addr);
    addr.house_number = 42;

    TEST_ASSERT_EQUAL_INT(42, address_get_house_number(&addr));
}

/* UT-004: Bug-regression -- house_number and street_number must be
   independent fields, unlike the original prototype's getStreetNumber()
   which incorrectly returned houseNumber. */
void test_UT_004_get_street_number_differs_from_house_number(void)
{
    Address addr;
    address_init(&addr);
    addr.house_number = 42;
    addr.street_number = 7;

    TEST_ASSERT_EQUAL_INT(42, address_get_house_number(&addr));
    TEST_ASSERT_EQUAL_INT(7, address_get_street_number(&addr));
    TEST_ASSERT_NOT_EQUAL_INT(address_get_house_number(&addr), address_get_street_number(&addr));
}

/* UT-005: address_get_city returns the correct value. */
void test_UT_005_get_city_positive(void)
{
    Address addr;
    address_init(&addr);
    strcpy(addr.city, "Metropolis");

    TEST_ASSERT_EQUAL_STRING("Metropolis", address_get_city(&addr));
}

/* UT-006: address_get_town_name returns the correct value. */
void test_UT_006_get_town_name_positive(void)
{
    Address addr;
    address_init(&addr);
    strcpy(addr.town_name, "Springfield");

    TEST_ASSERT_EQUAL_STRING("Springfield", address_get_town_name(&addr));
}

/* UT-007: address_get_house_number NULL safety. */
void test_UT_007_get_house_number_null_safety(void)
{
    TEST_ASSERT_EQUAL_INT(0, address_get_house_number(NULL));
}

/* UT-008: address_get_street_number NULL safety. */
void test_UT_008_get_street_number_null_safety(void)
{
    TEST_ASSERT_EQUAL_INT(0, address_get_street_number(NULL));
}

/* UT-009: address_get_city NULL safety. */
void test_UT_009_get_city_null_safety(void)
{
    TEST_ASSERT_EQUAL_STRING("", address_get_city(NULL));
}

/* UT-010: address_get_town_name NULL safety. */
void test_UT_010_get_town_name_null_safety(void)
{
    TEST_ASSERT_EQUAL_STRING("", address_get_town_name(NULL));
}

/* UT-011: address_display with a NULL pointer must not crash. */
void test_UT_011_display_null_safety(void)
{
    address_display(NULL);
    TEST_PASS();
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_UT_001_address_init_positive);
    RUN_TEST(test_UT_002_address_init_null_safety);
    RUN_TEST(test_UT_003_get_house_number_positive);
    RUN_TEST(test_UT_004_get_street_number_differs_from_house_number);
    RUN_TEST(test_UT_005_get_city_positive);
    RUN_TEST(test_UT_006_get_town_name_positive);
    RUN_TEST(test_UT_007_get_house_number_null_safety);
    RUN_TEST(test_UT_008_get_street_number_null_safety);
    RUN_TEST(test_UT_009_get_city_null_safety);
    RUN_TEST(test_UT_010_get_town_name_null_safety);
    RUN_TEST(test_UT_011_display_null_safety);

    return UNITY_END();
}
