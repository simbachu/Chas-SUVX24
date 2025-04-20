#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

std::mutex mtx;
std::condition_variable cv;
bool turn = true;

//


int our_number {0};

void print_even( const int target ) {
    while ( turn == true ) {
        std::lock_guard<std::mutex> l ( mtx );
        if ( our_number <= target && our_number % 2 == 0 ) {
            std::cout << our_number << " ";

            if ( our_number == target ){
                turn = false;
            } else { 
                ++our_number;
            }        
        }
    }
}

void print_odd( const int target ) {
    while ( turn == true ) {
        std::lock_guard<std::mutex> l ( mtx );
        if ( our_number <= target && our_number % 2 != 0 ) {
            std::cout << our_number << " ";
            
            if ( our_number == target ){
                turn = false;
            } else { 
                ++our_number;
            }
        }
    }
}


int main () {
    int n = 15;

    std::thread t1(print_even, n);
    std::thread t2(print_odd,  n);
        
    t1.join();
    t2.join();
}