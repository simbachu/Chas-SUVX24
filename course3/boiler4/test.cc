#include <thread>
#include <mutex>
#include <chrono>
#include <queue>

static std::mutex lock;
static std::condition_variable update;

void setup(std::vector<std::string> *destination){
    std::vector<std::string> names {"Adam", "Bertil", "Ceasar", "David"};
}

void worker(std::vector<std::string> *destination, std::queue<std::string> *queue){
    std::unique_lock<std::mutex> u_lock(lock);
    while (1) {
        update.wait(u_lock, [queue]{return queue->size() > 0;});
        destination->push_back(std::move(queue->front()));
        queue->pop();
    }
}

int main(){
    std::queue<std::string> work_queue;
    std::unique_lock<std::mutex> u_lock(lock);
    std::vector<std::thread> workers;
    while ( workers.size() < 3 ){
        while(!u_lock){
            u_lock.try_lock();
        }
        u_lock.unlock();
        workers.emplace_back(worker);
    }

}