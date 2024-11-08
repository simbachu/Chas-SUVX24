#include <algorithm>
#include "ShoppingCart.h"

ShoppingCart::ShoppingCart() : products{} {

}

void ShoppingCart::add_to_cart(StockKeepingUnit SKU) {
    
    auto it = std::find(products.begin(), products.end(), SKU);
    
    if(it != products.end()){
        *++it;
    } else {
        products.push_back(++SKU);
    }

}

double ShoppingCart::total(const Customer& customer){
    double sum = {};
    for ( auto sku : products ){
        sum += sku.get_unit_price() * sku.get_quantity();
    }

    sum = customer.applyDiscount(sum);

    return sum;
}