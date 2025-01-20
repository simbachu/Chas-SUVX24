#include <random>
#include <chrono>
#include <iostream>
#include "Bank.h"

BankAccount::BankAccount( int amount ) : balance {amount} {

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

const int Bank::register_account(){
    std::lock_guard<std::mutex> guard_accounts(l_acc);
    int account_number = idx;
    ++idx;
    std::shared_ptr<BankAccount> new_account = std::make_shared<BankAccount>();
    accounts[account_number] = std::move(new_account);
 
    return account_number;
}

void BankThread::client( Bank* bank){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> random_amount (100, 250); 
    std::uniform_int_distribution<int> d6 (1,6);

    std::this_thread::sleep_for(std::chrono::nanoseconds(random_amount(gen)));

    for ( auto& acc : bank->accounts ){
        if ( d6(gen) > 4 ){
            if ( d6(gen) > 3 ){
                acc.second->deposit(random_amount(gen));
            } else {
                int withdraw_amount = random_amount(gen);
                if (acc.second->get_balance() > withdraw_amount){
                    acc.second->withdraw(withdraw_amount);
                }
            }
        }
    }
}

void Bank::run(){
    int last_acc {0};
    while ( accounts.size() < 5 ){
        last_acc = register_account();
        accounts[last_acc]->deposit(300);
    }


    while ( clients.size() < 10 ){
        clients.push_back(std::thread(BankThread::client, this));
    }
}

void Bank::stop(){
    for ( auto& c : clients ){
        c.join();
    }

    for ( const auto& acc : accounts ){
        std::cout << "Account #" << acc.first << " balance: " << acc.second->get_balance() << '\n';
    }
}