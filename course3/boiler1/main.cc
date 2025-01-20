#include <iostream>
#include "Bank.h"


int main(){
    Bank bank;
    bank.run();
    std::cerr << "Sleeping main thread..." << '\n';
    std::this_thread::sleep_for(std::chrono::seconds(10));
    bank.stop();
}