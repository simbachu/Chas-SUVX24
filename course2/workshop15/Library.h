#ifndef CHAS_BOOK_LIBRARY_H
#define CHAS_BOOK_LIBRARY_H

#include "Book.h"
#include <map>
#include <optional>
#include <vector>

struct SearchResult {
    std::vector<unsigned> book_id;
};

class Library {

    private:
    std::map<unsigned, Book> books;
    std::map<unsigned, unsigned> stock;
    unsigned max_id() const;

    public:
    Library();

    std::optional<std::vector<unsigned>> search(std::string query);
    
    const Book& look_up (unsigned id);

    friend std::ostream& operator << (
        std::ostream&, const Library&);

    Library& operator += (Book&& new_book);
    Library& operator -= (unsigned id);
};

#endif