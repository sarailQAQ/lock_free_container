//
// Created by sarail on 9/19/23.
//
#include "../sl_set.h"
#include "../skiplist.h"


#include "atomic"
#include "thread"
#include "cstdlib"
#include "ctime"
#include "iostream"
#include "cstdio"
#include <chrono>

std::atomic<int> read_count, write_count;

inline int key_gen() {
    return int (random() % 100000ll);
}

void rand_write(int round, sl_set<int>& set) {
    int n = round;
    auto start = std::chrono::system_clock::now();
    while (round--) {
        set.insert(key_gen());
//        write_count++;
    }
    auto end = std::chrono::system_clock::now();   //结束时间
    double time_use = (double )(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    std::cout << "write op: " << 1000 * time_use / double(n) << " μs/op\n";
}

void rand_read(int round, sl_set<int>& set) {
    int n = round;

    auto start = std::chrono::system_clock::now();
    while (round--) {
        set.find(key_gen());
//        read_count++;
    }
    auto end = std::chrono::system_clock::now();   //结束时间
    double time_use = (double )(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

    std::cout << "read op: " << 1000  * time_use / double(n) << " μs/op\n";
}


int main() {
    srand(time(nullptr));


     sl_set<int> slist;

    int n = 500000, write_thread = 2, read_thread = 8;
    auto start = std::chrono::system_clock::now();

    std::vector<std::thread> threads;

    for (int i = 0; i < write_thread; i++) {
        std::thread w(rand_write, n, std::ref(slist));
        threads.push_back(std::move(w));
    }

    for (int i = 0; i < read_thread; i++) {
        std::thread r(rand_read, n, std::ref(slist));
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

