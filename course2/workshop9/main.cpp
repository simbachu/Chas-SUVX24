#include <iostream>
#include <vector>
#include "Product.h"
#include "Customer.h"
#include "ShoppingCart.h"

int main(){
    int UID_pool {};
    std::vector<Product> produce {};
    
    ShoppingCart cart;

    for (int i = 0; i < 5; i++){
        produce.push_back(Product("Banana", 0.5+i, UID_pool++));
    }
    for (int i = 0; i < 5; i++){
        produce.push_back(Product("Apple", (i*0.2)+0.1, UID_pool++));
    }

    for (auto p : produce){
        cart.add_to_cart(StockKeepingUnit(p, 3));
    }

    std::vector<Customer*> cust {};
    cust.push_back(new RegularCustomer());
    cust.push_back(new MemberCustomer());
    cust.push_back(new VIPCustomer());

    for ( auto c : cust ){
        std::cout << "Customer: " << cart.total(*c) << std::endl;
    }
}