#ifndef ACCOUNT_H
#define ACCOUNT_H

typedef struct {
    int id;
    double balance;
} Account;

void init_account(Account* acc, int id, double initial_balance);
void deposit(Account* acc, double amount);
int withdraw(Account* acc, double amount);

#endif