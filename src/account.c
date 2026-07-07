#include "account.h"

#include <stdio.h>

void init_account(Account* acc, int id, double initial_balance) {
    acc->id = id;
    acc->balance = initial_balance;
}

void deposit(Account* acc, double amount) {
    if (amount > 0) {
        acc->balance += amount;
        printf("Deposited: %.2f\n", amount);
    }
}

int withdraw(Account* acc, double amount) {
    if (amount > 0 && amount <= acc->balance) {
        acc->balance -= amount;
        printf("Withdrawn: %.2f\n", amount);
        return 1;
    }
    printf("Insufficient funds!\n");
    return 0;
}