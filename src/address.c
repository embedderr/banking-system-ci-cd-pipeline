/**
 * @file address.c
 * @brief Implementation of the address module.
 */

#include "address.h"

#include <stdio.h>
#include <string.h>

void address_init(Address *addr) {
    if (addr == NULL) {
        return;
    }

    addr->house_number = 0;
    addr->street_number = 0;
    addr->town_name[0] = '\0';
    addr->city[0] = '\0';
}

void address_display(const Address *addr) {
    if (addr == NULL) {
        return;
    }

    printf("\t\t\t\tHouse#: %d\n", addr->house_number);
    printf("\t\t\t\tTown Name: %s\n", addr->town_name);
    printf("\t\t\t\tStreet#: %d\n", addr->street_number);
    printf("\t\t\t\tCity: %s\n", addr->city);
}

int address_get_house_number(const Address *addr) {
    if (addr == NULL) {
        return 0;
    }
    return addr->house_number;
}

int address_get_street_number(const Address *addr) {
    if (addr == NULL) {
        return 0;
    }
    return addr->street_number;
}

const char *address_get_city(const Address *addr) {
    if (addr == NULL) {
        return "";
    }
    return addr->city;
}

const char *address_get_town_name(const Address *addr) {
    if (addr == NULL) {
        return "";
    }
    return addr->town_name;
}
