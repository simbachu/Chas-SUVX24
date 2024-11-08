#ifndef CHAS_PRODUCT_H
#define CHAS_PRODUCT_H

#include <string>

class Product {
    private:
    std::string name;
    int UID;
    double price;

    public:
    Product(std::string, double, int);
    friend bool operator==(const Product&, const Product&);
    friend bool operator!=(const Product&, const Product&);

    std::string get_name () const;
    void rename(std::string);
    int get_UID() const;
    double get_price() const;
    void set_price(double);
};


class StockKeepingUnit {
    private: 
    Product item;
    int quantity;

    public:
    
    StockKeepingUnit(Product, int);

    friend bool operator==(const StockKeepingUnit&, const StockKeepingUnit&);
    friend bool operator!=(const StockKeepingUnit&, const StockKeepingUnit&);

    StockKeepingUnit& operator+= (int);
    StockKeepingUnit& operator-=(int);
    StockKeepingUnit& operator*= (int);
    StockKeepingUnit& operator/=(int);
    StockKeepingUnit& operator++();
    StockKeepingUnit operator++(int);
    StockKeepingUnit& operator--();
    StockKeepingUnit operator--(int);

    int get_quantity() const;
    void set_quantity(int);
    double get_unit_price() const;
    void update(Product);
};


#endif