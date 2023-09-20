//
// Created by sarail on 9/20/23.
//

#ifndef LOCK_FREE_CONTAINER_LOCK_FREE_QUEUE_H
#define LOCK_FREE_CONTAINER_LOCK_FREE_QUEUE_H

#include "atomic"
#include "memory"
#include "exception"

template<typename T>
class lock_free_queue {
private:
    struct node {
        std::shared_ptr<T> data;
        std::atomic<node *> next;

        explicit node(T const &data_) :
                data(std::make_shared<T>(data_)) {}
    };

    std::atomic<node *> head = nullptr;
    std::atomic<node *> tail = nullptr;
    std::atomic<size_t> _size = 0;
public:
    void push(T const &data) {
        node *new_node = new node(data);
        node *_ptr = nullptr;
        if (!head.compare_exchange_weak(_ptr, new_node) || !tail.compare_exchange_weak(_ptr, new_node))
            _ptr = nullptr;
        else
            return;
        node *old_tail = tail.load();
        while (old_tail &&
               !old_tail->next.compare_exchange_weak(_ptr, new_node)) {
            old_tail = tail.load();
            _ptr = nullptr;
        }
        tail.compare_exchange_weak(old_tail, new_node);

        _size++;
    }

    std::shared_ptr<T> pop() {
        node *old_head = head.load();
        if (old_head == nullptr) return nullptr;
        while (old_head &&
               !head.compare_exchange_weak(old_head, old_head->next)) {
            old_head = head.load();
        }

        _size--;
        return old_head ? old_head->data : std::shared_ptr<T>();
    }

    T front() {
        if (head == nullptr) throw std::exception();

        auto h = head.load();
        return h->data;
    }

    size_t size() {
        return _size.load();
    }

    bool empty() {
        return head.load() == nullptr;
    }
};

#endif //LOCK_FREE_CONTAINER_LOCK_FREE_QUEUE_H
