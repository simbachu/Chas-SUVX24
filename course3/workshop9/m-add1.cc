#include <random>
#include <chrono>
#include <iostream>

constexpr size_t matrix_size = 10000;
int a[matrix_size][matrix_size];
int b[matrix_size][matrix_size];
int dest[matrix_size][matrix_size];

void init( ){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(-1000, 1000);

    for ( size_t i = 0; i < matrix_size; i++ ){
        for ( size_t j = 0; j < matrix_size; j++ ){
            a[i][j] = dist(gen);
            b[i][j] = dist(gen);
            dest[i][j] = 0;
        }
    }
}

void m_add_row (  ){
    for ( size_t i = 0; i < matrix_size; i++ ){
        for ( size_t j = 0; j < matrix_size; j++ ){
            dest[i][j] = a[i][j] + b[i][j];
        }
    }   
}

void m_add_col (  ){
    for ( size_t i = 0; i < matrix_size; i++ ){
        for ( size_t j = 0; j < matrix_size; j++ ){
            dest[j][i] = a[j][i] + b[j][i];
        }
    }   
}

int main(){
    {
        init();    
        const auto before = std::chrono::system_clock::now();
        m_add_row();
        const auto after = std::chrono::system_clock::now();
        const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(after-before);
        std::cout << "Row: " << time << '\n';
    }
    {
        init();    
        const auto before = std::chrono::system_clock::now();
        m_add_col();
        const auto after = std::chrono::system_clock::now();
        const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(after-before);
        std::cout << "Column: " << time << '\n';
    }

}