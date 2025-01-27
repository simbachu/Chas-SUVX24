#include <random>
#include <chrono>
#include <iostream>
#include "Bank.h"

BankAccount::BankAccount( int new_account_number ) : account_number {new_account_number} {

}

void BankAccount::deposit(int amount) {
    std::lock_guard<std::mutex> balance_lock(l);
    balance += amount;
}

void BankAccount::withdraw(int amount) {
    std::lock_guard<std::mutex> balance_lock(l);
    balance -= amount;
}

const int BankAccount::get_balance() const{
    return balance;
}

const std::shared_ptr<BankAccount> Bank::new_account(){
    std::lock_guard<std::mutex> guard_accounts(l_acc);
    int account_number = idx;
    ++idx;
    auto new_account = std::make_shared<BankAccount>(account_number);
    accounts.push_back(new_account);
 
    return new_account;
}

void BankThread::client( Bank* bank){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> random_amount (100, 250); 
    std::uniform_int_distribution<int> d6 (1,6);

    while( bank->running() ){
        std::this_thread::sleep_for(std::chrono::nanoseconds(random_amount(gen)));
        int first_number {0};
        int second_number {0};
        while (first_number == second_number){
            first_number = d6(gen)-1;
            second_number = d6(gen)-1;
        }
        if ( std::max(first_number, second_number) < bank->customers.size() ){
            Transaction t {
                bank->customers[first_number].get_account(),
                bank->customers[second_number].get_account(),
                random_amount(gen)
            };
            {
                std::lock_guard<std::mutex> q_l (bank->l_acc);
                bank->work_queue.push(t);
            }
            bank->queue_s.notify_one();
        }
    }
}

void BankThread::worker( Bank* bank){
    std::unique_lock<std::mutex> q_l(bank->l_acc);
    while(bank->running()){    
        bank->queue_s.wait(q_l, !bank->running());
        auto t = bank->work_queue.front();
        bank->work_queue.pop();
        q_l.unlock();
        if ( auto from = t.from.lock() ){
            if ( t.amount < from->get_balance() ){
                if ( auto to = t.to.lock() ){
                    from->withdraw(t.amount);
                    to->deposit(t.amount);
                }
                else {
                    std::lock_guard<std::mutex> l (bank->l_log);
                    bank->logger.log(); //missing recipient
                }
            } else {
                std::lock_guard<std::mutex> l (bank->l_log);
                bank->logger.log(); //missing balance
            }
        } else {
                    std::lock_guard<std::mutex> l (bank->l_log);
                    bank->logger.log(); //missing giver
        }
    }
}

void Bank::run(){
    std::vector<std::string> c_names = {"Adam", "Bertil", "Ceasar", "David", "Erik", "Filip"};
    std::lock_guard<std::mutex> c_g(l_acc);
    for ( const auto name : c_names ){
        customers.emplace_back(name);
    }
    for ( auto& customer : customers ){
        customer.register_account(new_account());
        if (auto acc = customer.get_account().lock() ){
            acc->deposit(300);
            }
    }

    while ( workers.size() < 4 ){
        workers.push_back(std::thread(BankThread::worker, this));
    }

    while ( clients.size() < 10 ){
        clients.push_back(std::thread(BankThread::client, this));
    }

}

const bool Bank::running() const{
    return !done;
}

void Bank::stop(){
    done = true;

    for ( auto& c : clients ){
        c.join();
    }

    for ( const auto& acc : accounts ){
        std::cout << "Account #" << acc->get_account_number() << " balance: " << acc->get_balance() << '\n';
    }
}