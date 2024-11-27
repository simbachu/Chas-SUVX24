#ifndef CHAS_WS_PRODUCTDB_H
#define CHAS_WS_PRODUCTDB_H

#include "json.hpp"

class Date{
    private:
    int y {1970};
    int m {1};
    int d {1};

    public:
    Date() = default;
    Date(int yyyy, int mm, int dd);
    static Date from_string(std::string);
    static Date from_ints(int yyyy, int mm, int dd);

    friend std::ostream& operator << (std::ostream&, const Date&);
};

struct Product {
    std::string product_name;
    std::string product_description;
    double unit_price;
};

struct OrderRow {
    unsigned product_id;
    unsigned amount;
};

struct Order {
    Date date;
    unsigned customer_id;
    std::vector<OrderRow> rows;
/*     Order() = default;
    Order(Date &d, unsigned c_id, std::vector<OrderRow> && r) */;
};

class Order_History {
    private:
    std::map<unsigned, Product> stockkeeping_units;
    std::map<unsigned, Order> orders;

    public:
    Order_History();

    friend std::ostream& operator << ( std::ostream&, const Order_History& );

};


#endif