#include "Book.h"

#include <iostream>

std::ostream& operator << (std::ostream& os, const Book& b){
    os << b.author << ": " << b.title << " (" << b.isbn << ")";
    return os;
}

std::ostream& operator << (std::ostream& os, const Book b){
    os << b.author << ": " << b.title << " (" << b.isbn << ")";
    return os;
}