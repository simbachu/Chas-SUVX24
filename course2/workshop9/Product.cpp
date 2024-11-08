#include <string>
#include "Product.h"

Product::Product(std::string product_name, double product_price, int UID) :
        name{product_name},
        price{product_price},
        UID{UID} {
}

bool operator==(const Product& lhs, const Product& rhs){
    if( lhs.get_UID() == rhs.get_UID() ){
        return true;
    } else {
        return false;
    }
}

bool operator!=(const Product& lhs, const Product& rhs){
    return !(lhs == rhs);
}

std::string Product::get_name() const {
    return name;
}

void Product::rename(std::string new_name){
    name = std::move(new_name);
}

int Product::get_UID() const{
    return UID;
}

double Product::get_price() const {
    return price;
}

void Product::set_price(double new_price){
    price = new_price;
}


StockKeepingUnit::StockKeepingUnit(Product item, int initial_stock) : item{item}, quantity{initial_stock}{

}

bool operator==(const StockKeepingUnit& lhs, const StockKeepingUnit& rhs){
    return lhs.item == rhs.item;
}
bool operator!=(const StockKeepingUnit&lhs, const StockKeepingUnit& rhs){
    return !(lhs.item == rhs.item);
}

StockKeepingUnit& StockKeepingUnit::operator+=(int val){
    this->set_quantity(val + quantity);
    return *this;
}

StockKeepingUnit& StockKeepingUnit::operator-=(int val){
    this->set_quantity(val - quantity);
    return *this;
}

StockKeepingUnit& StockKeepingUnit::operator*=(int val){
    this->set_quantity(val * quantity);
    return *this;
}

StockKeepingUnit& StockKeepingUnit::operator/=(int val){
    
    if (val != 0){
        this->set_quantity(quantity / val);
    }
    return *this;
}

StockKeepingUnit& StockKeepingUnit::operator++(){
    this->set_quantity(quantity++);
    return *this;
}
StockKeepingUnit StockKeepingUnit::operator++(int){
    StockKeepingUnit old = *this;
    this->set_quantity(quantity++);
    return old;
}
StockKeepingUnit& StockKeepingUnit::operator--(){
    this->set_quantity(quantity--);
    return *this;
}
StockKeepingUnit StockKeepingUnit::operator--(int){
    StockKeepingUnit old = *this;
    this->set_quantity(quantity--);
    return old;
}

int StockKeepingUnit::get_quantity() const {
    return quantity;
}

void StockKeepingUnit::set_quantity(int val){
    if (val > quantity){
        quantity = 0;
    } else {
        quantity = val;
    }

}

double StockKeepingUnit::get_unit_price() const{
    return this->item.get_price();
}

