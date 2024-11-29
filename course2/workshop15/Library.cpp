#include <iostream>
#include <fstream>

#include "Library.h"
#include "Book.h"
#include "json.hpp"

Library::Library(){
    std::ifstream book_file;
    book_file.exceptions(std::ifstream::badbit);
    try {
        book_file.open("books.json");
    } catch(...) {
        std::cerr << "Couldn't open file";
        throw;
    }
    nlohmann::json book_json;
    try {
        book_file >> book_json;
    } catch(nlohmann::json::exception e){
        std::cerr << "json read error";
        throw e;
    }
    book_file.close();
    for ( const auto & b : book_json ){
        books.emplace(b["id"].get<unsigned>(), Book { b["title"].get<std::string>(), b["author"].get<std::string>(), b["isbn"].get<std::string>() });
        stock.emplace(b["id"].get<unsigned>(), b["copies"].get<unsigned>());
    }
}

unsigned Library::max_id() const {
    auto max_it = books.rbegin();
    return (*max_it).first;
}

std::optional<std::vector<unsigned>> Library::search(std::string query){
    std::vector<unsigned> results {};
    for ( const auto& [book_id, book] : books ){
        if (book.author.find(query) != std::string::npos ){
            results.emplace_back(book_id);
        } else if (book.title.find(query) != std::string::npos ) {
            results.emplace_back(book_id);
        } else if (book.isbn.find(query) != std::string::npos ) {
            results.emplace_back(book_id);}     
    }
    if (results.size() > 0){
        return results;
    } else {
        return std::nullopt;
    }
}

const Book& Library::look_up (unsigned id){
    try{
        auto iter = books.at(id);
    } catch (const std::out_of_range& e){
        std::cerr << e.what();
        throw e;
    }
    return books.at(id);
}

std::ostream& operator << ( std::ostream& os, const Library& l ){
    for ( const auto [book_id, book] : l.books){
        os << std::to_string(l.stock.at(book_id)) << "x " << book.author << ": " << book.title << " (ISBN: " << book.isbn << ")" << '\n';
    }
    return os;
}

Library& Library::operator+=(Book&& new_book){
    unsigned new_id = {max_id() + 1};
    books.emplace(new_id, std::move(new_book));
    stock.emplace(new_id, unsigned{0});

    return *this;
}

Library& Library::operator-=(unsigned id){
    try{
        auto iter = books.at(id);
    } catch (const std::out_of_range& e){
        std::cerr << e.what();
        throw e;
    }
    books.erase(id);
    stock.erase(id);

    return *this;
}