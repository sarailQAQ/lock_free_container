//
// Created by sarail on 9/18/23.
//

#ifndef LOCK_FREE_CONTAINER_ATOMIC_SET_H
#define LOCK_FREE_CONTAINER_ATOMIC_SET_H

#include <iostream>
#include <atomic>
#include <functional>
#include "set"

const int NUM_BUCKETS = 500;

template <typename T, typename Hash = std::hash<T>>
struct Node {
    T value;
    Node* next;
    Node(T val) : value(val), next(nullptr) {}
};

template <typename T, typename Hash = std::hash<T>>
class courrent_set {
private:
    std::vector<std::atomic<Node<T>*>> buckets;

    int hash(int key) {
        return key % NUM_BUCKETS;
    }

public:
    atomic_set() : buckets(NUM_BUCKETS) {}

    bool insert(const T& key) {
        int h = hash(key);
        Node<T>* newNode = new Node<T>(key);

        while (true) {
            Node<T>* head = buckets[h].load();
            newNode->next = head;

            if (buckets[h].compare_exchange_weak(head, newNode)) {
                return true;
            }
        }
    }

    bool contains(const T& key) {
        int h = hash(key);
        Node<T>* current = buckets[h].load();

        while (current) {
            if (current->value == key) {
                return true;
            }
            current = current->next;
        }

        return false;
    }

    bool find(const T& key) {
        return contains(key);
    }
};

#endif //LOCK_FREE_CONTAINER_ATOMIC_SET_H
