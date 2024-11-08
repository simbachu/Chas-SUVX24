#ifndef CHAS_SHOPPINGCART_H
#define CHAS_SHOPPINGCART_H
#include "Customer.h"
#include "Product.h"
#include <vector>


class ShoppingCart {
    private:
    std::vector<StockKeepingUnit> products;

    public:
    ShoppingCart();
    void add_to_cart(StockKeepingUnit);
    double total(const Customer&);
};

#endif