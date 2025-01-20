#include <random>
#include <chrono>
#include <iostream>
#include "Bank.h"

BankAccount::BankAccount( int amount ) : balance {amount} {

}

void BankAccount::deposit(int amount) noexcept {
    balance += amount;
}

void BankAccount::withdraw(int amount) {
}

const int Bank::register_account(){
    std::lock_guard<std::mutex> guard_accounts(l_acc);
    int account_number = idx;
    ++idx;
    accounts.emplace(account_number, BankAccount(300));
    return account_number;
}

void Bank::client(){
    std::random_device rd;
    std::mt19937 gen(rd);
    std::uniform_int_distribution<int> random_amount (100, 250); 
    std::uniform_int_distribution<int> d6 (1,6);

    std::this_thread::sleep_for(std::chrono::nanoseconds(random_amount(gen)));

    for ( auto& acc : accounts ){
        if ( d6(gen) > 4 ){
            if ( d6(gen) > 3 ){
                acc.second.deposit(random_amount(gen));
            } else {
                int withdraw_amount = random_amount(gen);
                if (acc.second.getBalance() > withdraw_amount){
                    acc.second.withdraw(withdraw_amount);
                }
            }
        }
    }
}

void Bank::run(){
    int noop {0};
    while ( accounts.size() < 5 ){
        noop = register_account();
    }

    while ( clients.size() < 10 ){
        clients.push_back(std::thread(client));
    }
}

void Bank::stop(){
    for ( auto& c : clients ){
        c.join();
    }

    for ( const auto& acc : accounts ){
        std::cout << "Account " << acc.first << " balance: " << acc.second.getBalance() << '\n';
    }
}