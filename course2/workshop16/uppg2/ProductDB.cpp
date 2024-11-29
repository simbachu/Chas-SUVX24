#include "ProductDB.h"

#include <format>
#include <string>
#include <iostream>
#include <iomanip>
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
    // prints YYYY-MM-DD
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

std::string Date::to_string() const{
    std::string out{};
    out += std::to_string(y);
    out += "-";
    if ( m < 10){
        out += "0";
    }
    out += std::to_string(m);
    out += "-";
    if (d < 9){
        out += "0";
    }
    out += std::to_string(d);;

    return out;
}


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

    std::ifstream customer_file;
    customer_file.exceptions(std::ifstream::badbit);
    try {
        customer_file.open("customers.json");
    } catch(...) {
        std::cerr << "Couldn't open file";
        throw;
    }
    nlohmann::json customer_json;
    try {
        customer_file >> customer_json;
    } catch(nlohmann::json::exception e){
        std::cerr << "json read error";
        throw e;
    }
    customer_file.close();
    for ( const auto & p : customer_json ){
        customers.insert(
            { p["customer_id"].get<unsigned>(),
             {
                p["customer_name"].get<std::string>(),
                p["customer_email"].get<std::string>()
             }
            }
        );
    }
}

Order_History& Order_History::operator +=( Product && new_product ){
    const auto last_item = stockkeeping_units.rbegin();
    int max_key = (*last_item).first;
    stockkeeping_units.emplace(++max_key, std::move(new_product));

    return *this;
}

Order_History& Order_History::operator +=( Order && new_order ){
    const auto last_item = orders.rbegin();
    int max_key = (*last_item).first;
    orders.emplace(++max_key, std::move(new_order));

    return *this;
}

Order_History& Order_History::operator +=( Customer && new_customer ){
    const auto last_item = customers.rbegin();
    int max_key = (*last_item).first;
    customers.emplace(++max_key, std::move(new_customer));

    return *this;
}

std::ostream& operator << ( std::ostream& os, const Order_History& o_h ){
    std::cout.setf(std::ios::fixed);
    std::cout.setf(std::ios::showpoint);
    for( const auto & [k,v] : o_h.orders ){
        double order_total {};
        std::string order = std::format(
            "Order no. {} ({}): {} ({})\n", 
            k, v.date.to_string(), o_h.customers.at(v.customer_id).name, o_h.customers.at(v.customer_id).email
        );

        os << std::setprecision(2);
        os << order;

        for ( const auto & r : v.rows){
            double row_total {o_h.stockkeeping_units.at(r.product_id).unit_price*r.amount};
            std::string name = o_h.stockkeeping_units.at(r.product_id).product_name;
            double price = o_h.stockkeeping_units.at(r.product_id).unit_price;
            unsigned amount = r.amount;
            std::string row = std::format( "{:30} ${:^5.2f} * {:<4}: ${:8>.2f}\n", name, price, amount, row_total );
            os << row;
            order_total += row_total;
        }
        os << "--------------------" << '\n';
        std::string total = std::format("Subtotal: {:>5.2f}\nTax: {:>5.2f}\nTotal: {:>5.2f}\n", order_total*0.8, order_total*0.2, order_total);
        os << total << std::endl;
    }
    return os;
}