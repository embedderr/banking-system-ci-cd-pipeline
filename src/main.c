#include <stdio.h>

#include "account.h"

int main() {
    printf("=== Banking System Started ===\n");

    Account myAccount;
    init_account(&myAccount, 1001, 5000.0);

    deposit(&myAccount, 1500.0);
    withdraw(&myAccount, 2000.0);

    printf("Final Balance: %.2f\n", myAccount.balance);
    printf("=== Banking System Ended ===\n");

    return 0;
}