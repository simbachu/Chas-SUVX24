#include "ContactDB.h"
#include <string>
#include <iostream>
#include <fstream>

/*
ContactInfo
Helper function for printing with << operator
*/

std::ostream& operator << (std::ostream& os, const ContactInfo c){
    std::string out_string = c.first_name;
    if ( c.last_name != "" ){
        out_string += " " + c.last_name;
    }
    if ( c.birth_year != "" ){
        out_string += " (b. " + c.birth_year + ") ";
    }
    if ( c.email != "" ){
        out_string += " " + c.email + " ";
    }
    if (c.phone != ""){
        out_string += " tel. " + c.phone;
    }
    
    os << out_string;
    return os;
}

/*
ContactInfoDB
Handles the contact info retrieval and storage.
*/

ContactInfoDB::ContactInfoDB() {
    std::ifstream file;
    file.exceptions(std::ifstream::badbit);
    try {
         file.open("contactdb.json");
    } catch (const std::ifstream::failure& e){
        std::cerr << "Couldn't open file" << '\n';
        throw e;
    }
    nlohmann::json db;
    file >> db;

    records.reserve(db.size());
    for ( const auto & record : db ){
        records.emplace_back( ContactInfo {
            record["id"].get<unsigned>(),
            record["first_name"].get<std::string>(),
            record["last_name"].get<std::string>(),
            record["birth_year"].get<std::string>(),
            record["email"].get<std::string>(),
            record["phone"].get<std::string>()}
        );
    }

    file.close();

    autoincrement_key = records.back().id;
}

ContactInfo ContactInfoDB::get_record_by_id(unsigned id){
    auto found_record = std::find_if(records.begin(), records.end(),
        [&](const auto& record ){ return record.id == id; });
    if(found_record != records.end()){
        return *found_record;
    } else {
        throw std::invalid_argument("No record with that ID");
    }
}

void ContactInfoDB::create_record(ContactInfo record){
    record.id = ++autoincrement_key;    
    try{
        records.emplace_back(std::move(record));
    } catch(...){
        throw;
    }
}

void ContactInfoDB::update_record(unsigned id, ContactInfo new_data){
    auto update = std::find_if(records.begin(), records.end(),
        [&](const auto& record ){ return record.id == id; });
    if ( update != records.end() ){
        *update = new_data;
    } else {
        throw std::invalid_argument("No record with that ID");
    }
}

void ContactInfoDB::delete_record(unsigned id){
    auto removed_record = std::erase_if(records,
        [&](const auto& record ){ return record.id == id; });
    if ( removed_record > 0 ){
        return;        
    } else {
        throw std::invalid_argument("No record with that ID");
    }
}

const ContactInfo& ContactInfoDB::read_record(unsigned id) const {
    auto found_record = std::find_if(records.begin(), records.end(),
        [&](const auto& record ){ return record.id == id; });
    if ( found_record != records.end() ){
        return *found_record;
    } else {
        throw std::invalid_argument("No record with that ID");
    }
}

std::vector<unsigned> ContactInfoDB::get_indexes() const {
    std::vector<unsigned> out{};
    for ( const auto & record : records ){
        try{
            out.emplace_back(record.id);
        } catch(...){
            throw;
        }
    }
    return out;
}

void ContactInfoDB::save_records(){
    nlohmann::json updated_json;
    for ( const auto & record : records){
        updated_json.push_back({
            {"id", record.id},
            {"first_name", record.first_name},
            {"last_name", record.last_name},
            {"birth_year", record.birth_year},
            {"email", record.email},
            {"phone", record.phone}
        });
    }
    std::ofstream output("contactdb.json");
    output.exceptions(std::fstream::badbit);
    try{
        output << updated_json.dump(4);
    } catch (...) {
        std::cerr << "Couldn't write to file";
        throw;
    }

    std::cout << "Changes saved to contactdb.json" << std::endl;
}

std::ostream& operator <<(std::ostream& os, const ContactInfoDB& db){
    for(const auto & record : db.records){
        os << record.id << ". " << record << '\n';
    }
    return os;
}

/*
Contacts
User interface for contact list via ContactInfoDB
*/

Contacts::Contacts() : db{}{

}

Contacts::operator bool(){
    return running;
}

void Contacts::menu(){
    std::string input_string {};

    std::cout << " --- MENU: ---  " << '\n';
    std::cout << " Select mode:" << '\n';
    std::cout << "1. List contacts" << '\n';
    std::cout << "2. Create new contact" << '\n';
    std::cout << "3. Update existing contact" << '\n';
    std::cout << "0: Exit" << '\n';
    std::cout << ">>  ";

    std::getline(std::cin, input_string);
    int option = input_string[0] - '0';
    while(option < 0 || option > 3 ){
            std::getline(std::cin, input_string);
            option = input_string[0] - '0';
    }
    switch (option){
        case 1:
        print_contacts('a');
        print_contacts('b');
        break;
        case 2: 
        new_contact();
        break;
        case 3:
        update_contact();
        break;
        case 4: 
        remove_contact();
        default:
        exit();
        break;
    }
}

void Contacts::print_contacts(char sort){
    std::vector record_ids = db.get_indexes();
    switch(sort){
        case 'a': {
        auto lastname_pp = std::partition(record_ids.begin(), record_ids.end(), 
            [this](const auto & record){ 
                return db.get_record_by_id(record).last_name.size() > 0; } );
        std::sort(record_ids.begin(), lastname_pp, 
            [this](const auto& lhs, const auto& rhs){
                return db.get_record_by_id(lhs).last_name < db.get_record_by_id(rhs).last_name;
          });
        std::cout << '\n' << " --- CONTACTS (Ascending alphabetical) ---  " << '\n';
        break;
        }
        case 'b':{
        auto erased = std::erase_if(record_ids, 
            [this](const auto & record){ return db.get_record_by_id(record).birth_year == ""; });
        std::sort(record_ids.begin(), record_ids.end(), [this](const auto& lhs, const auto& rhs){
                return db.get_record_by_id(lhs).birth_year < db.get_record_by_id(rhs).birth_year;
          });
        std::cout << '\n' << " --- CONTACTS (Oldest to newest) ---  " << '\n';
        break;
        }
    }


    for ( const auto & r : record_ids ){
        std::cout << db.read_record(r) << '\n';
    }
}

void Contacts::new_contact(){
    ContactInfo new_contact {};
    std::cout << " --- NEW CONTACT ---" << std::endl;
    std::cout << "Enter first name: ";
    std::getline(std::cin, new_contact.first_name);

    std::cout << "Enter last name: ";
    std::getline(std::cin, new_contact.last_name);
    
    std::cout << "Enter birth year: ";
    std::getline(std::cin, new_contact.birth_year);

    std::cout << "Enter email address: ";
    std::getline(std::cin, new_contact.email);

    std::cout << "Enter phone number: ";
    std::getline(std::cin, new_contact.phone);
    std::cout << std::endl;

    std::cout << "Adding record: " << new_contact << std::endl;
    db.create_record(new_contact);
    changes_made = true;
}

void Contacts::update_contact(){
    std::string input_string {};
    std::vector possible_records = db.get_indexes();
    std::cout << " --- UPDATE CONTACT --- " << '\n';
    std::cout << db;
    std::cout << "Select record to update by id:  >>";

    std::getline(std::cin, input_string);
    int option = input_string[0] - '0';
    if ( option == 0 ){
        return;
    } else {
        while( std::find(possible_records.begin(), possible_records.end(), option) == possible_records.end() ){
                std::cout << "Invalid id. Try again:  >>";
                std::getline(std::cin, input_string);
                option = input_string[0] - '0';
        }
    }
    auto to_update = db.get_record_by_id(option);

    std::cout << "Enter updated first name [" << to_update.first_name << "]: ";
    std::getline(std::cin, input_string);
    if ( input_string.size() != 0 ){
        to_update.first_name = input_string;
        changes_made = true;
    }

    std::cout << "Enter updated last name [" << to_update.last_name << "]: ";
    std::getline(std::cin, input_string);
    if ( input_string.size() != 0 ){
        to_update.last_name = input_string;
        changes_made = true;
    }

    std::cout << "Enter updated birth year [" << to_update.birth_year << "]: ";
    std::getline(std::cin, input_string);
    if ( input_string.size() != 0 ){
        to_update.birth_year = input_string;
        changes_made = true;
    }

    std::cout << "Enter updated email address [" << to_update.email << "]: ";
    std::getline(std::cin, input_string);
    if ( input_string.size() != 0 ){
        to_update.email = input_string;
        changes_made = true;
    }

    std::cout << "Enter updated phone number: [" << to_update.phone << "] ";
    std::getline(std::cin, input_string);
    if ( input_string.size() != 0 ){
        to_update.phone = input_string;
        changes_made = true;
    }
    std::cout << std::endl;

    db.update_record(option, to_update);
}

void Contacts::remove_contact(){
    std::string input_string {};
    std::vector possible_records = db.get_indexes();
    std::cout << " --- DELETE CONTACT --- " << '\n';
    std::cout << db;
    std::cout << "Select record to remove by id:  >>";

    std::getline(std::cin, input_string);
    int option = input_string[0] - '0';
    if ( option == 0 ){
        return;
    } else {
        while( std::find(possible_records.begin(), possible_records.end(), option) == possible_records.end() ){
                std::cout << "Invalid id. Try again:  >>";
                std::getline(std::cin, input_string);
                option = input_string[0] - '0';
        }
        db.delete_record(option);
        changes_made = true;
    }
}

void Contacts::exit(){
    std::string input_string {};

    if (changes_made){
        std::cout << "You have unsaved changes." << std::endl;        
        std::cout << "Do you want to save? y/n   >> ";

        while(input_string[0] != 'y' && input_string[0] != 'n' ){
                std::getline(std::cin, input_string);
        }
        switch (input_string[0]){
            case 'y':
            db.save_records();
            break;
            default:
            std::cout << "Exiting without saving changes.";
            break;
        }
    }
    running = false;
}