#include <vector>
#include <chrono>
#include <random>
#include <iostream>

void m_add ( const std::vector<std::vector<int>> &a,
             const std::vector<std::vector<int>> &b,
                std::vector<std::vector<int>> &c ){

    for ( auto i = 0; i < a.size(); i++ ){
        for ( auto j = 0; j < a[i].size(); j++ ){
            c[i][j] = a[i][j] + b[i][j];
        }
    } 
}

void m_add_colwise ( const std::vector<std::vector<int>> &a,
             const std::vector<std::vector<int>> &b,
                std::vector<std::vector<int>> &c ){

    for ( auto i = 0; i < a.size(); i++ ){
        for ( auto j = 0; j < a[i].size(); j++ ){
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1000);
    size_t matrix_size = 10000;
    std::vector<std::vector<int>> a(matrix_size);
    for ( auto &v : a ){
        v.reserve(matrix_size);
        for ( auto i = 0; i < matrix_size; i++ ){
            v.push_back(dist(gen));
            }    
    }
    std::vector<std::vector<int>> b(matrix_size);
    for ( auto &v : b ){
        v.reserve(matrix_size);
        for ( auto i = 0; i < matrix_size; i++ ){
            v.push_back(dist(gen));
        }
    }    
    std::vector<std::vector<int>> c(matrix_size);
    for ( auto &v : c ){
        v.reserve(matrix_size);
        for ( auto n = 0; n < matrix_size; n++ ){
            v.push_back(0);
        }
    }

    {    
        const auto before = std::chrono::system_clock::now();
        m_add(a, b, c);
        const auto after = std::chrono::system_clock::now();

        const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);

        std::cout << '\a' << "It took " << time << '\n'; //C++20
    }
}