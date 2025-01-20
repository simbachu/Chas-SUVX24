#include "Bank.h"


int main(){
    Bank bank;
    bank.run();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    bank.stop();
}