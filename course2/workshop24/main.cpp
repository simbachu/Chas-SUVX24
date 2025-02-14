#include <string>
#include <atomic>
#include <random>
#include <chrono>
#include <thread>
#include <iostream>
#include <syncstream>

std::atomic<bool> running;

void print_toggle(){
    std::osyncstream syncout(std::cout);
    bool toggle = false;
    std::vector<std::string> words { "Off", "On" };
    while( running ){
        toggle = !toggle;
        syncout << "Toggle thread: " << words[toggle] << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(15));
    }
};

void print_random_temperatures(){
    std::osyncstream syncout(std::cout);
    std::random_device rd;
    std::mt19937 gen { rd() };
    std::uniform_real_distribution<> temp_range(5.0, 15.0);
    while( running ){
        syncout << "Temperature thread: " << ((temp_range(rd) * (9.0/5.0)) + 32.0) << " degrees F" << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

void print_concurrency(){
    std::osyncstream syncout(std::cout);
    std::string concurrency { "Concurrency" };
    auto it = concurrency.begin();

    while( running ){
            syncout << "Concurrency thread: ";
            ++it;
            for ( auto wc = concurrency.begin(); wc != it; ++wc ){
                syncout << *wc;
            }
            syncout << '\n';
            if ( it == concurrency.end()){
                it = concurrency.begin();
            }
            
            std::this_thread::sleep_for(std::chrono::seconds(6));
    }

}

int main(){
    running = true;
    
    std::thread toggle (print_toggle);
    std::thread temp (print_random_temperatures);
    std::thread concurrency (print_concurrency);

    std::this_thread::sleep_for(std::chrono::seconds(60));
    running = false;

    toggle.join();
    temp.join();
    concurrency.join();
}