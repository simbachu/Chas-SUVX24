#include <string>
#include <memory>
#include <vector>
#include <iostream>

struct Warehouse;

struct Product{
    std::string name;
    std::shared_ptr<Warehouse> loc;
};

struct Warehouse{
    std::string loc;
    std::vector<std::weak_ptr<Product>> products;
};


int main(){
    std::shared_ptr<Warehouse> centrallager = std::make_shared<Warehouse>();
    centrallager->loc = "Centrallager";
    centrallager->products = {};

    std::vector<std::shared_ptr<Product>> products { 
        std::make_shared<Product>(), 
        std::make_shared<Product>() };

    for ( auto &product : products ){
        product->loc = centrallager;
        product->loc->products.push_back(product);
    }

    std::cout << centrallager->products.front().expired() << '\n';
    products.clear();
    std::cout << centrallager->products.front().expired() << '\n';
}