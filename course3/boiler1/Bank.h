#ifndef CHAS_BANK_H
#define CHAS_BANK_H

#include <atomic>
#include <map>
#include <mutex>
#include <thread>
#include <memory>
#include <chrono>

class BankAccount {
    private:
    int account_number = 0;
    int balance = 0;
    std::mutex l;

    public:
    BankAccount() = default;
    BankAccount( int account_number );
    void deposit ( int amount );
    void withdraw ( int amount );
    const int get_balance() const;
    const int get_account_number() const;
};

class Customer {
    std::string name;
    std::shared_ptr<BankAccount> account;

    public:
    Customer( std::string name );
    void register_account ( std::shared_ptr<BankAccount> );
    std::weak_ptr<BankAccount> get_account();

};

struct Transaction {
    std::weak_ptr<BankAccount> from;
    std::weak_ptr<BankAccount> to;
    const int amount;
    std::chrono::time_point<std::chrono::system_clock> when = std::chrono::system_clock::now();
};


struct LogMessage{
    
};

class Reporter{
    public:
    void log();
};

class Bank {
    private:
    bool done = false;

    public:
    Reporter logger;
    std::mutex l_log;
    std::atomic<int> idx = 100;
    std::vector<Customer> customers;
    std::vector<std::shared_ptr<BankAccount>> accounts;
    std::mutex l_acc;
    std::vector<std::thread> clients;
    std::vector<std::thread> workers;
    std::condition_variable queue_s;
    std::queue<Transaction> work_queue;

    Bank() = default;
    const std::shared_ptr<BankAccount> new_account (  );
    const BankAccount& get_account ( int acc_number ) const;
    const bool running() const;

    void run();
    void stop();
};

namespace BankThread{
    void client( Bank* bank );
    void worker( Bank* bank );
};

#endif