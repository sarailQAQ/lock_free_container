//
// Created by sarail on 9/19/23.
//
#include "set"
#include "atomic"
#include "thread"
#include "cstdlib"
#include "ctime"
#include "iostream"
#include "cstdio"
#include <chrono>
#include "vector"
#include "mutex"
#include "unordered_set"
std::mutex mu;

inline int key_gen() {
    return int (random() % 10000ll);
}

void rand_write(int round, std::unordered_multiset<int>& set) {
    int n = round;
    auto start = std::chrono::system_clock::now();
    while (round--) {
        std::lock_guard lock(mu);
        set.insert(key_gen());
//        write_count++;
    }
    auto end = std::chrono::system_clock::now();   //结束时间
    double time_use = (double )(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    std::cout << "write op: " << 1000 * time_use / double(n) << " μs/op\n";
}

void rand_read(int round, std::unordered_multiset<int>& set) {
    int n = round;

    auto start = std::chrono::system_clock::now();
    while (round--) {
        std::lock_guard lock(mu);
        set.find(key_gen());
//        read_count++;
    }
    auto end = std::chrono::system_clock::now();   //结束时间
    double time_use = (double )(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    std::cout << "read op: " << 1000  * time_use / double(n) << " μs/op\n";
}


int main() {
    std::cout << "mutex_set_bench\n";

    srand(time(nullptr));


    std::unordered_multiset<int> st;

    int n = 500000, write_thread = 2, read_thread = 0;
    auto start = std::chrono::system_clock::now();

    std::vector<std::thread> threads;

    for (int i = 0; i < write_thread; i++) {
        std::thread w(rand_write, n, std::ref(st));
        threads.push_back(std::move(w));
    }

    for (int i = 0; i < read_thread; i++) {
        std::thread r(rand_read, n, std::ref(st));
        threads.push_back(std::move(r));
    }

    for (auto & thread : threads) thread.join();

    auto end = std::chrono::system_clock::now();   //结束时间
    double time_use = (double )(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1000;
    long long tot = n * (read_thread + write_thread);
    std::cout << "total op: " << tot << "\n";
    std::cout<<"time = "<< time_use <<" s\n";
    std::cout << "throughput = " << int (double(tot) / time_use) << " op/s\n";


    return 0;
}

