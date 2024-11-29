#include "Book.h"
#include "Library.h"

#include <iostream>

int main(){
    Library lib;
    lib += Book{ 
        "The annotated Gulliver's travel", 
        "Jonathan Swift", 
        "9788726039269" 
    };
    std::cout << "Do we have something by Swift? " << std::endl;
    auto res = lib.search("Swift");
    if ( res.has_value() ){
        for ( const auto& book_id : res.value() ){
            auto book = lib.look_up(book_id);
            std::cout << book.author << ": " << book.title << " (ISBN: " << book.isbn << ")" << '\n';
        }
    } else {
        std::cout << "No result for query";
    }

    std::cout << "I meant Taylor Swift? " << std::endl;
    res = lib.search("Taylor Swift");
    if ( res.has_value() ){
        for ( const auto& book_id : res.value() ){
            auto book = lib.look_up(book_id);
            std::cout << book.author << ": " << book.title << " (ISBN: " << book.isbn << ")" << '\n';
        }
    } else {
        std::cout << "No result for query";
    }
}