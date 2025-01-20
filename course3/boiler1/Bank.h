#ifndef CHAS_BANK_H
#define CHAS_BANK_H

#include <atomic>
#include <map>
#include <mutex>
#include <thread>
#include <memory>

class BankAccount {
    private:
    int balance = 0;
    std::mutex l;

    public:
    BankAccount() = default;
    BankAccount( int amount );
    void deposit ( int amount );
    void withdraw ( int amount );
    const int get_balance() const;
};

class Bank {
    public:
    std::atomic<int> idx = 100;
    std::map<int, std::shared_ptr<BankAccount>> accounts;
    std::mutex l_acc;
    std::vector<std::thread> clients;

    Bank() = default;
    const int register_account (  );
    const BankAccount& get_account( int acc_number ) const;

    void run();
    void stop();
};

namespace BankThread{
    void client(Bank* bank);
};

#endif