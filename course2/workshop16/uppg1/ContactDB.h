#ifndef CHAS_WS_CONTACTDB_H
#define CHAS_WS_CONTACTDB_H

#include "json.hpp" //nlohmann
#include <iostream>
#include <string>

struct ContactInfo {
    unsigned id {};
    std::string first_name {};
    std::string last_name {};
    std::string birth_year {};
    std::string email {};
    std::string phone {};
};

class ContactInfoDB {
    private:
    std::vector<ContactInfo> records;
    unsigned autoincrement_key {};

    public:
    ContactInfoDB();

    friend std::ostream& operator <<(std::ostream& os, const ContactInfoDB& db);

    ContactInfo get_record_by_id(unsigned id);
    void create_record(ContactInfo);
    const ContactInfo& read_record(unsigned id) const;
    std::vector<unsigned> get_indexes() const;
    void update_record(unsigned id, ContactInfo);
    void delete_record(unsigned id);
    void save_records();
};

class Contacts {
    private:
    ContactInfoDB db;
    bool running;

    public:
    Contacts();

    explicit operator bool();
    
    void menu();
    void print_contacts(char sort);
    void new_contact();
    void update_contact();
    void remove_contact();
    void exit();
};

#endif