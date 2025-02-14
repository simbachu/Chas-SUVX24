#include <vector>
#include <memory>
#include <iostream>


void f( int n ){
    int arr[n]{};

    for( int i = 0; i < n; i++ ){
        arr[i] = i+1;
        
        std::cout << arr[i] << " ";
    }

    std::cout << std::endl;
}

auto f_two ( int n ){
    auto ret = std::make_unique<int[]>(n);

    for( int i = 0; i < n; i++ ){
        ret[i] = i+1;
        
        std::cout << ret[i] << " ";
    }

    std::cout << std::endl;

    return ret;
}

auto f_three( int n ){
    std::vector<int> arr;
    arr.reserve(n);
    for( int i = 0; i < n; i++ ){
        arr[i] = i+1;
        
        std::cout << arr[i] << " ";
    }
    std::cout << '\n';

    return arr.data();
}

int main (){
    int times {4};

    f(times);

    auto arr = f_two(times);

    f(arr[3]);

    {
        int* arr_two = f_three(arr[3]);
        for( int i = 0; i < times; i++ ){
//            arr_two[i] = i+1;
            std::cout << arr_two[i] << " ";
        }
        std::cout << '\n';

        delete[](arr_two);
    }
}