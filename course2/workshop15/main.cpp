#include "json.hpp"
#include "Book.h"

#include <iostream>
#include <fstream>

int main(){
    std::ifstream f("books.json");
    if(!f){
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }
}