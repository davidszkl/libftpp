#include "libftpp.hpp"
#include <thread>

void thread_function(int arg) {
    std::cout << arg << std::endl;
}

int main() {
    Pool<std::thread> thread_pool(5);
    std::deque<Pool<std::thread>::Object> pool_thread_queue;
    std::deque<std::thread> thread_queue;

    pool_thread_queue.push_back(thread_pool.acquire(thread_function, 1));
    pool_thread_queue.push_back(thread_pool.acquire(thread_function, 2));
    pool_thread_queue.push_back(thread_pool.acquire(thread_function, 3));
    pool_thread_queue.push_back(thread_pool.acquire(thread_function, 4));
    pool_thread_queue.push_back(thread_pool.acquire(thread_function, 5));

    for (size_t i = 0; i < pool_thread_queue.size(); i++) {
        auto& obj = pool_thread_queue[i];
        if (obj->joinable()) {
            obj->join();
        }
    }

    thread_pool.resize(10);
}