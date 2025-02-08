#include <chrono>
#include <random>
#include <memory>
#include <iostream>

constexpr static size_t ARRAY_SZ = 11000;

static_assert( ARRAY_SZ < (SIZE_T_MAX / ARRAY_SZ));


void init( int *a, int *b, int *dest ){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(-1000, 1000);

    if(a != nullptr && b != nullptr && dest != nullptr){
        for ( size_t i = 0; i < ARRAY_SZ; i++ ){
            for ( size_t j = 0; j < ARRAY_SZ; j++ ){
            a[(i * ARRAY_SZ) + j] = dist(gen);
            b[(i * ARRAY_SZ) + j] = dist(gen);
            dest[(i * ARRAY_SZ) + j] = 0;
            }
        }
    }
}

void m_add_row ( int *a, int *b, int *dest ){
    if (a != nullptr && b != nullptr && dest != nullptr) {
        for ( size_t i = 0; i < ARRAY_SZ; i++ ){
        for ( size_t j = 0; j < ARRAY_SZ; j++ ){
            dest[(i * ARRAY_SZ) + j] = a[(i * ARRAY_SZ) + j] + b[(i * ARRAY_SZ) + j];
            }
        }   
    }
}

int main(){
    std::cout << "Array dimension: " << ARRAY_SZ << " * " << ARRAY_SZ << " = " << ARRAY_SZ*ARRAY_SZ << " items." << '\n';
    std::unique_ptr<int> arr_a;
    std::unique_ptr<int> arr_b;
    std::unique_ptr<int> arr_c;
    
    try{
        arr_a = std::make_unique<int>(ARRAY_SZ * ARRAY_SZ);
    } catch ( std::bad_alloc ){
        std::cerr << "Bad allocation!";
    };
    try{
        arr_b = std::make_unique<int>(ARRAY_SZ * ARRAY_SZ);
    } catch ( std::bad_alloc ){
        std::cerr << "Bad allocation!";
    };    
    try{
        arr_c = std::make_unique<int>(ARRAY_SZ * ARRAY_SZ);
    } catch ( std::bad_alloc ){
        std::cerr << "Bad allocation!";
    };
    if(arr_a && arr_b && arr_c){
        {
            std::cout << "Initializing arrays... " << '\n'; 
            const auto before = std::chrono::system_clock::now();
            init(arr_a.get(), arr_b.get(), arr_c.get());  
            const auto after = std::chrono::system_clock::now();
            const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(after-before);
            std::cout << "Initializing arrays: " << time << '\n'; 
        }
        const auto before = std::chrono::system_clock::now();
        m_add_row(arr_a.get(), arr_b.get(), arr_c.get());
        const auto after = std::chrono::system_clock::now();
        const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(after-before);
        if(arr_c.get()[50] == arr_a.get()[50] + arr_b.get()[50]){
            std::cout << "Row: " << time << '\n';            
        }
        else {
            std::cout << "Math error" << '\n';
        };

    }
}