#include <atomic>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <utility>
#include <vector>

constexpr size_t CHUNK_SIZE = 30;

struct Chunk
{
    size_t len;
    char data[CHUNK_SIZE];
}; //c++


std::queue<Chunk> requests;
std::mutex req_mutex; //mutex lives with the data it synchronizes


constexpr size_t NUM_WORKERS = 4;

/* Synchronization */
// Put your global synchronization variables/objects here
//
//
std::atomic<bool> producing = true;
std::atomic<unsigned int> total_sum = 0;


unsigned int processData(Chunk &chunk)
{
    unsigned int local_sum = 0;

    for (size_t i = 0; i < chunk.len; ++i) {
        local_sum += (unsigned int)(chunk.data[i]);
    }

    return local_sum;
}

void producer()
{
    Chunk chunk;
    std::ifstream dataStream("data.bin", std::ios::binary);

    while (dataStream)
    {
        dataStream.read(chunk.data, CHUNK_SIZE);
        chunk.len = dataStream.gcount();

        std::lock_guard<std::mutex> lg(req_mutex);
        std::cout << "Data: " << std::string(chunk.data) << "\n";

        requests.emplace(chunk);
    }

    producing = false;
}

void consumer()
{
    do {
        std::lock_guard<std::mutex> lg(req_mutex);
        if (!requests.empty())
        {
            std::cout << "Processing data: " << std::string(requests.front().data) << "\n";
            total_sum += processData(requests.front());
            requests.pop();
        }
    } while (producing || !requests.empty());
}

int main()
{
    std::cout << "Accumulating byte value (0 - 255) sum.\n";

    std::thread prodThread(producer);
    
    {
        std::vector<std::thread> consumer_pool;
        for (int n = 0; n < NUM_WORKERS; ++n)
            consumer_pool.emplace_back(consumer);

        for (std::thread &worker : consumer_pool)
            worker.join();
    }

    prodThread.join();
    std::cout << "Sum: " << total_sum << "\n";
}