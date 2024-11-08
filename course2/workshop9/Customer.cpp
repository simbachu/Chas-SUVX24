#include "Customer.h"

Customer::Customer(){}

Customer::~Customer(){}

double RegularCustomer::applyDiscount(double total) const {
    return total;
}

RegularCustomer::RegularCustomer() {}

MemberCustomer::MemberCustomer() : discount_rate{0.1} {}

VIPCustomer::VIPCustomer() : discount_rate{0.2} {}

double MemberCustomer::applyDiscount(double total) const{
    return total * (1.0 - discount_rate);
}

double VIPCustomer::applyDiscount(double total) const{
    return total * (1.0 - discount_rate);
}