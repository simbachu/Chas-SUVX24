#ifndef CHAS_BANK_H
#define CHAS_BANK_H

#include <atomic>
#include <map>
#include <mutex>
#include <thread>

class BankAccount {
    private:
    std::atomic<int> balance = 0;

    public:
    BankAccount() = default;
    BankAccount( int amount );
    void deposit ( int amount ) noexcept;
    void withdraw ( int amount );
    const int getBalance() const;
};

class Bank {
    private:
    std::atomic<int> idx = 100;
    std::map<int, BankAccount> accounts;
    std::mutex l_acc;
    std::vector<std::thread> clients;

    public:
    Bank() = default;
    const int register_account (  );
    const BankAccount& get_account( int acc_number ) const;

    void client();

    void run();
    void stop();
};

#endif