#include <vector>
#include <iostream>
#include <algorithm>

class A {
    int a {1};
    int b {2};
    const int c {3};

    public:
    auto oop ();
};

auto A::oop(){
    std::vector<int> aaaaa;
    
    for( int i = 0; i < c; i++ ){
        aaaaa.push_back(i);
    }    

    return aaaaa;
}

int main(){
    A a;
    int *oops = (int *) &a;
    std::cout << oops[2] << '\n';
    oops[2] = 1;
    std::cout << oops[2] << '\n';

    auto k = a.oop();

    for ( auto& mm : k ){
        mm += 1;
        std::cout << "   " << mm << '\n';
    }
}