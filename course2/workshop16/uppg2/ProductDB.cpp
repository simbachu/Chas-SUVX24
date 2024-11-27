#include "ProductDB.h"

#include <string>
#include <iostream>
#include <fstream>

Date::Date(int yyyy, int mm, int dd) : y{yyyy}, m{mm}, d{dd}{

}

Date Date::from_string(std::string yyyy_mm_dd){
    
    int yyyy{ std::stoi(yyyy_mm_dd.substr(0, 4))};
    int mm  { std::stoi(yyyy_mm_dd.substr(5, 2))};
    int dd  { std::stoi(yyyy_mm_dd.substr(8, 2))};

    try{
        Date ret = Date::from_ints(yyyy, mm, dd);
        return ret;
    } catch (...){
        std::cerr << "Error parsing date string: " << yyyy_mm_dd << '\n';
        throw;
    }
}

Date Date::from_ints(int yyyy, int mm, int dd){
    unsigned feb {28};
    if(yyyy % 4 == 0){
        if(yyyy % 100 == 0){
            if(yyyy % 400 == 0){
                feb = 29; //2000
            }
        } else {
            feb = 29;
        }
    }
    std::map<unsigned, unsigned> month_lengths {
        {1,31}, {2,feb}, {3,31}, {4,30}, {5,31}, {6,30},
        {7,31}, {8,31}, {9,30}, {10,31}, {11,30}, {12,31}
    };
    if( (yyyy < 1700 || yyyy > 2100)  || (mm < 0 || mm > 12 ) || ( dd < 1 || dd > 31 )){
        std::cerr << "Wrong date format: " << yyyy << " " << mm << " " << dd << '\n';
        throw std::invalid_argument("Misformed date");    
    } else if( dd > month_lengths[mm] ){
        std::cerr << "Day number is too high for the month: " << dd << " in " << mm << '\n';
        std::cerr << "Expected " << month_lengths[mm] << " days max in month " << mm << '\n';
        throw std::invalid_argument("Misformed date");
    }
    return Date(yyyy, mm, dd);
}

std::ostream& operator<<(std::ostream& os, const Date& d ){
    os << std::to_string(d.y);
    os << "-";
    if ( d.m < 10){
        os << "0";
    }
    os << std::to_string(d.m);
    os << "-";
    if (d.d < 9){
        os << "0";
    }
    os << std::to_string(d.d);;

    return os;
}

/* Order::Order(Date &d, unsigned c_id, std::vector<OrderRow>&& r) : 
    date{std::move(d)}, customer_id{c_id}, rows{std::move(r)} {

} */


Order_History::Order_History() {
    std::ifstream product_file;
    product_file.exceptions(std::ifstream::badbit);
    try {
        product_file.open("products.json");
    } catch(...) {
        std::cerr << "Couldn't open file";
        throw;
    }
    nlohmann::json product_json;
    try {
        product_file >> product_json;
    } catch(nlohmann::json::exception e){
        std::cerr << "json read error";
        throw e;
    }
    product_file.close();
    for ( const auto & p : product_json ){
        stockkeeping_units.insert(
            { p["product_id"].get<unsigned>(),
             {
                p["product_name"].get<std::string>(),
                p["product_description"].get<std::string>(), 
                p["unit_price"].get<double>()
             }
            }
        );
    }
    std::ifstream order_file;
    order_file.exceptions(std::ifstream::badbit);
    try {
        order_file.open("orders.json");
    } catch(...) {
        std::cerr << "Couldn't open file";
        throw;
    }
    nlohmann::json order_json;
    try {
        order_file >> order_json;
    } catch(nlohmann::json::exception e){
        std::cerr << "json read error";
        throw e;
    }
    order_file.close();
    for ( const auto & o : order_json ){
        std::vector<OrderRow> row_v{};
        for (const auto & r : o["order_rows"]){
            row_v.emplace_back(OrderRow{r["product_id"].get<unsigned>(), r["amount"].get<unsigned>()});
        }
        unsigned o_id = o["order_number"].get<unsigned>();
        unsigned c_id = o["customer_id"].get<unsigned>();
        Order new_order {Date::from_string(o["date"].get<std::string>()), c_id, row_v};
        orders.insert_or_assign(c_id, new_order);
    }
}

std::ostream& operator << ( std::ostream& os, const Order_History& o_h ){

    for( const auto & [k,v] : o_h.orders ){
        os << "Order no." << k << " (" << v.date << "): " << '\n';
        os << " Cust. nr " << v.customer_id << " " << '\n';
        for ( const auto & r : v.rows){
            os << "  " << o_h.stockkeeping_units.at(r.product_id).product_name << " $" << o_h.stockkeeping_units.at(r.product_id).unit_price << " * " << r.amount << ": $" << o_h.stockkeeping_units.at(r.product_id).unit_price*r.amount << '\n';
        }
    }
    return os;
}