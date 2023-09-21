//
// Created by sarail on 9/21/23.
//
#include "../sl_set.h"
#include "set"
#include "assert.h"
#include "iostream"

int main() {
    srand(time(nullptr));

    sl_set<int> slSet;
    std::set<int> st;

    int n = 10000000;
    for (int i = 0; i < n; i++) {
        int op = rand() % 2;
        int key = rand() % 1000;

        if (op == 0) {
            // write
            auto r1 = st.insert(key);
            auto r2 = slSet.insert(key);

            assert(r1.second == r2.second);
        } else {
            auto r1 = st.find(key);
            auto r2 = slSet.find(key);

            if (r1 == st.end()) assert(r2 == slSet.end());
            else
                assert(*r1 == *r2);
        }
    }

    std::cout << "sl_set test passed\n";
}