#include <random>
#include <chrono>
#include <iostream>
#include "Bank.h"

BankAccount::BankAccount( int new_account_number ) : account_number {new_account_number} {

}

void BankAccount::deposit(int amount) {
    balance += amount;
}

void BankAccount::withdraw(int amount) {
    balance -= amount;
}

const int BankAccount::get_balance() const{
    return balance;
}

const int BankAccount::get_account_number() const{
    return account_number;
}

Customer::Customer( std::string name_in ) : name {name_in}{

}

void Customer::register_account( std::shared_ptr<BankAccount> acc ){
    account = acc;
}

const std::weak_ptr<BankAccount> Customer::get_account() const{
    return account;
}

const std::shared_ptr<BankAccount> Bank::new_account(){
    std::lock_guard<std::mutex> guard_accounts(lock);
    int account_number = idx;
    ++idx;
    auto new_account = std::make_shared<BankAccount>(account_number);
    accounts.push_back(new_account);
 
    return new_account;
}

void Bank::log(LogMessage msg){
    log_queue.push(msg);
}

void Bank::run(){
    std::unique_lock<std::mutex> queue_lock(lock);
    std::cerr << "Starting logging thread: " << '\n';
    queue_lock.unlock();
    std::thread logging(BankThread::logger, this);
    while(!queue_lock){
        queue_lock.try_lock();
    }
    {std::cerr << "Creating customers: " << '\n';

    std::vector<std::string> c_names = {
    "Adam", "Bertil", "Ceasar", "David", "Erik", "Filip"};
    for ( const auto name : c_names ){
        customers.emplace_back(name);
    }
    for ( auto& customer : customers ){
        customer.register_account(new_account());
        if (auto acc = customer.get_account().lock() ){
            acc->deposit(300);
        }
    }}
    std::cerr << "Starting worker threads: " << '\n';
    queue_lock.unlock();

    while ( workers.size() < 1 ){
        workers.push_back(std::thread(BankThread::worker, this));
    }

    while ( clients.size() < 8 ){
        clients.push_back(std::thread(BankThread::client, this));
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));
    {
        std::lock_guard<std::mutex> l(lock);
        taking_orders = false;
    }

    while(has_work()){
        transaction_available.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    };

    for ( auto& c : clients ){
        if (c.joinable()){
            c.join();
        }
    }

    logging.join();

    for ( const auto& acc : accounts ){
        std::cout << "Account #" << acc->get_account_number() << " balance: " << acc->get_balance() << '\n';
    }

}

const bool Bank::accepts_transactions() const noexcept{
    return taking_orders;
}

const bool Bank::has_work() const noexcept{
    return work_queue.size() > 0 || log_queue.size() > 0;
}

std::ostream& operator << ( std::ostream& os, const LogMessage msg ){
    std::string from_s = "_UNKNOWN_";
    std::string to_s = "_UNKNOWN_";
    std::string success_s = " successfully transfered ";
    std::string fail_s = " failed to transfer ";
    std::string out_s;
    msg.msg_t == message_type::Success ? out_s = success_s : fail_s;  
    if( auto from = msg.t.from.lock() ){
        from_s = from->get_account_number();
    }
    if( auto to = msg.t.to.lock() ){
        to_s = to->get_account_number();
    }
    os << " Log: " << from_s << out_s << msg.t.amount << "kr to " << to_s << '\n';
    return os;
}

void BankThread::logger(Bank* bank){
    std::unique_lock<std::mutex> queue_lock(bank->lock);
    while (true) {

    if (queue_lock){
        std::cerr << "Logging thread " <<  std::this_thread::get_id() << " started." << '\n';}
    bank->log_available.wait(queue_lock, 
            [bank]{return bank->log_queue.size() > 0;} );
    if (queue_lock){
        std::cerr << "Logging thread " <<  std::this_thread::get_id() << " awoken." << '\n';}
    auto msg = bank->log_queue.front();
    bank->log_queue.pop();
    std::cout << msg << '\n';
    queue_lock.unlock();
    }
}

void BankThread::client( Bank* bank){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> random_amount (100, 250); 
    std::uniform_int_distribution<int> d6 (1,6);

    while( bank->accepts_transactions() ){
        {
            std::lock_guard<std::mutex> queue_lock (bank->lock);
            std::uniform_int_distribution<int> customer_range (0, bank->customers.size()-1);

            int first_number {customer_range(gen)};
            int second_number {customer_range(gen)};
            while (first_number == second_number){
                first_number = customer_range(gen);
                second_number = customer_range(gen);
            }
            bank->work_queue.push({
                bank->customers.at(first_number).get_account(),
                bank->customers.at(second_number).get_account(),
                random_amount(gen)}
            );
        }
        bank->transaction_available.notify_one();
        std::this_thread::sleep_for(std::chrono::nanoseconds(random_amount(gen)));
    }
}

void BankThread::worker( Bank* bank){
    std::unique_lock<std::mutex> queue_lock(bank->lock);
    std::cerr << "Worker thread " << std::this_thread::get_id() << " started." << '\n';
    while(1){
        bank->transaction_available.wait(queue_lock, 
            [bank]{ return bank->work_queue.size() > 0;} );
        auto t = bank->work_queue.front();
        bank->work_queue.pop();
        
        if ( auto from = t.from.lock() ){
            if ( t.amount < from->get_balance() ){
                if ( auto to = t.to.lock() ){
                    from->withdraw(t.amount);
                    to->deposit(t.amount);
                    bank->log({message_type::Success, t}); //success
                }
                else {
                    bank->log({message_type::Missing_Account, t}); //missing recipient
                }
            } else {
                bank->log({message_type::Balance_Error, t}); //missing balance
            }
        } else {
            bank->log({message_type::Missing_Account, t}); //missing giver
        }
        queue_lock.unlock();
        bank->log_available.notify_one();
    }
}