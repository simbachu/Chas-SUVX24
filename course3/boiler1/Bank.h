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
    const std::weak_ptr<BankAccount> get_account() const;

};

struct Transaction {
    std::weak_ptr<BankAccount> from;
    std::weak_ptr<BankAccount> to;
    const int amount;
    std::chrono::time_point<std::chrono::system_clock> when = std::chrono::system_clock::now();
};

enum struct message_type {
    Success,
    Missing_Account,
    Balance_Error
};

struct LogMessage{
    message_type msg_t;
    Transaction t;
    std::chrono::time_point<std::chrono::system_clock> when = std::chrono::system_clock::now();
};

std::ostream& operator << ( std::ostream&, LogMessage );


class Bank {
    private:
    bool taking_orders = true;
    bool stop_work = false;

    public:
    std::optional<std::thread> logger_thread;
    std::atomic<int> idx = 100;
    std::vector<Customer> customers;
    std::vector<std::shared_ptr<BankAccount>> accounts;
    std::mutex lock;
    std::vector<std::thread> clients;
    std::vector<std::thread> workers;
    std::condition_variable transaction_available;
    std::condition_variable log_available;
    std::queue<Transaction> work_queue;
    std::queue<LogMessage> log_queue;

    const std::shared_ptr<BankAccount> new_account (  );
    const BankAccount& get_account ( int acc_number ) const;
    const bool accepts_transactions() const noexcept;
    const bool has_work() const noexcept;

    void log(LogMessage);
    void run();
    void stop();
};

namespace BankThread{
    void logger( Bank* bank );
    void client( Bank* bank );
    void worker( Bank* bank );
};

#endif