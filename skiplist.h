//
// Created by Sarail Tenma on 2023/9/16.
//

#ifndef LOCK_FREE_CONTAINER_SKIPLIST_H
#define LOCK_FREE_CONTAINER_SKIPLIST_H

#include <stddef.h>
#include <stdint.h>
#include <atomic>

#define SKIPLIST_MAX_LAYER (64)

struct skiplist_node;

typedef std::atomic<skiplist_node *> atm_node_ptr;
typedef std::atomic<bool> atm_bool;
typedef std::atomic<uint8_t> atm_uint8_t;
typedef std::atomic<uint16_t> atm_uint16_t;
typedef std::atomic<uint32_t> atm_uint32_t;

struct skiplist_node {
    atm_node_ptr *next;
    atm_bool is_fully_linked;
    atm_bool being_modified;
    atm_bool removed;
    uint8_t top_layer; // 0: bottom
    atm_uint16_t ref_count;
    atm_uint32_t accessing_next;
};

// *a  < *b : return neg
// *a == *b : return 0
// *a  > *b : return pos
typedef int skiplist_cmp_t(skiplist_node *a, skiplist_node *b, void *aux);

struct skiplist_raw_config {
    size_t fanout;
    size_t maxLayer;
    void *aux;
};

struct skiplist_raw {
    skiplist_node head;
    skiplist_node tail;
    skiplist_cmp_t *cmp_func;
    void *aux;
    atm_uint32_t num_entries;
    atm_uint32_t *layer_entries;
    atm_uint8_t top_layer;
    uint8_t fanout;
    uint8_t max_layer;
};

#ifndef _get_entry
#define _get_entry(ELEM, STRUCT, MEMBER)                              \
        ((STRUCT *) ((uint8_t *) (ELEM) - offsetof (STRUCT, MEMBER)))
#endif

void skiplist_init(skiplist_raw *slist,
                   skiplist_cmp_t *cmp_func);

void skiplist_free(skiplist_raw *slist);

void skiplist_init_node(skiplist_node *node);

void skiplist_free_node(skiplist_node *node);

size_t skiplist_get_size(skiplist_raw *slist);

skiplist_raw_config skiplist_get_default_config();

skiplist_raw_config skiplist_get_config(skiplist_raw *slist);

void skiplist_set_config(skiplist_raw *slist, skiplist_raw_config config);

int skiplist_insert(skiplist_raw *slist, skiplist_node *node);

int skiplist_insert_nodup(skiplist_raw *slist, skiplist_node *node);

skiplist_node *skiplist_find(skiplist_raw *slist, skiplist_node *query);

skiplist_node *skiplist_find_smaller_or_equal(skiplist_raw *slist,
                                              skiplist_node *query);

skiplist_node *skiplist_find_greater_or_equal(skiplist_raw *slist,
                                              skiplist_node *query);

int skiplist_erase_node_passive(skiplist_raw *slist,
                                skiplist_node *node);

int skiplist_erase_node(skiplist_raw *slist,
                        skiplist_node *node);

int skiplist_erase(skiplist_raw *slist, skiplist_node *query);

int skiplist_is_valid_node(skiplist_node *node);

int skiplist_is_safe_to_free(skiplist_node *node);

void skiplist_wait_for_free(skiplist_node *node);

void skiplist_grab_node(skiplist_node *node);

void skiplist_release_node(skiplist_node *node);

skiplist_node *skiplist_next(skiplist_raw *slist,
                             skiplist_node *node);

skiplist_node *skiplist_prev(skiplist_raw *slist,
                             skiplist_node *node);

skiplist_node *skiplist_begin(skiplist_raw *slist);

skiplist_node *skiplist_end(skiplist_raw *slist);

#endif
