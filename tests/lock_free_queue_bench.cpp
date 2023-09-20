//
// Created by sarail on 9/19/23.
//
#include "../lock_free_queue.h"


#include "atomic"
#include "thread"
#include "cstdlib"
#include "ctime"
#include "iostream"
#include "cstdio"
#include <chrono>
#include "vector"
#include "assert.h"

std::atomic<int> read_count, write_count;

inline int key_gen() {
    return int (random() % 100000ll);
}

void rand_write(int round, lock_free_queue<int>& queue) {
    int n = round;
    auto start = std::chrono::system_clock::now();
    while (round--) {
        queue.push(key_gen());
//        write_count++;
    }
    auto end = std::chrono::system_clock::now();   //结束时间
    double time_use = (double )(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    std::cout << "push op: " << 1000 * time_use / double(n) << " μs/op\n";
}

void rand_read(int round,  lock_free_queue<int>& queue) {
    int n = round;

    auto start = std::chrono::system_clock::now();
    while (round--) {
//        while (queue.empty());
        queue.pop();

//        read_count++;
    }
    auto end = std::chrono::system_clock::now();   //结束时间
    double time_use = (double )(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    std::cout << "pop op: " << 1000  * time_use / double(n) << " μs/op\n";
}


int main() {
    std::cout << "lock_free_queue bench\n";
    srand(time(nullptr));


    lock_free_queue<int> q;

    int n = 500000, write_thread = 1, read_thread = 0;
    auto start = std::chrono::system_clock::now();

    std::vector<std::thread> threads;

    for (int i = 0; i < write_thread; i++) {
        std::thread w(rand_write, n, std::ref(q));
        threads.push_back(std::move(w));
    }

    for (int i = 0; i < read_thread; i++) {
        std::thread r(rand_read, n, std::ref(q));
        threads.push_back(std::move(r));
    }

    for (auto & thread : threads) thread.join();

    auto end = std::chrono::system_clock::now();   //结束时间
    double time_use = (double )(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000;
    long long tot = n * (read_thread + write_thread);
    std::cout << "total op: " << tot << "\n";
    std::cout<<"time = "<< time_use <<" s\n";
    std::cout << "throughput = " << int (double(tot) / time_use) << " op/s\n";

    std::cout << q.size() << '\n';

    return 0;
}

