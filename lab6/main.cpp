/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <random>

const int BUFFER_SIZE = 10;
std::queue<int> buffer;
std::mutex mtx;
std::condition_variable bufferEmpty;

void producer() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 100);

    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        if (buffer.size() < BUFFER_SIZE) {
            int data = distribution(generator);
            buffer.push(data);
            std::cout << "Produced: " << data << std::endl;
            lock.unlock();
            bufferEmpty.notify_one();
        } else {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Подождать некоторое время, если буфер полный
        }
    }
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        bufferEmpty.wait(lock, []{ return !buffer.empty(); });
        int data = buffer.front();
        buffer.pop();
        lock.unlock();
        std::cout << "Consumed: " << data << std::endl;
    }
}

int main() {
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    return 0;
}
