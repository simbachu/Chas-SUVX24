#include "ProductDB.h"
#include <iostream>

int main (){
    Order_History orders;
    try{
        std::cout << orders;
    } catch (...) {
        std::cerr << ":O";
    }
}