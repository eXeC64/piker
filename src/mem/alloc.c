#include "kernel.h"

#include "mem/alloc.h"
#include "mem/frame.h"

/* 18 bytes */
typedef struct {
    uint32_t start;             /* start address in memory  */
    uint32_t size;              /* size covered by node     */
    uint32_t next_node;         /* address of next node     */
    uint16_t flags;             /* bit flags for node       */
} alloc_node_t;

/* bit masks for alloc_node_t.flags */
#define NODE_VALID 1
#define NODE_ALLOCATED 1<<1

/* 4096 bytes */
typedef struct {
    uint32_t frame;             /* address of frame being sub-allocated */
    uint32_t next_table;        /* address of next allocation table     */
    uint32_t start_node;
    alloc_node_t nodes[226];    /* array of allocation nodes            */
    uint32_t _padding[4];       /* padding, 4 + 4 + 4 + 18*226 + 16 = 4096  */
} alloc_table_t;

/* Base allocation table is statically declared, and page aligned */
alloc_table_t base_table __attribute__((aligned (4096)));

void mem_alloc_init() {
    frame_alloc(&(base_table.frame));
    base_table.next_table = NULL;
    base_table.start_node = (uint32_t)&(base_table.nodes[0]);

    for(uint32_t i = 0; i < 226; ++i) {
        base_table.nodes[i].flags = 0;
    }

    base_table.nodes[0].start = base_table.frame;
    base_table.nodes[0].size = 4096;
    base_table.nodes[0].next_node = NULL;
    base_table.nodes[0].flags = NODE_VALID; /* valid, but not used */
}

alloc_node_t* mem_get_best_node(alloc_table_t* table, uint32_t size) {
    alloc_node_t* best_node = NULL;

    if(table == NULL) {
        return NULL;
    }

    alloc_node_t* cur_node = (alloc_node_t*) table->start_node;

    while(cur_node != NULL) {

        /* check if node is valid */
        if(cur_node->flags & NODE_VALID) {

            /* check if this node is free */
            if(0 == (cur_node->flags & NODE_ALLOCATED)) {

                /* the node is valid and available, check the size */
                if(cur_node->size == size) {
                    return cur_node;
                }

                if(cur_node->size > size) {

                    /* see if this node is the best match so far */
                    if(best_node == NULL) {
                        best_node = cur_node;
                    } else if(best_node != NULL && best_node->size > cur_node->size) {
                        best_node = cur_node;
                    }
                }
            }

            /* walk on */
            cur_node = (alloc_node_t*) cur_node->next_node;

        } else {
            /* node not valid, stop walking */
            cur_node = NULL;
        }
    }

    return best_node;
}

alloc_node_t* mem_get_unused_node(alloc_table_t* table) {
    if(table == NULL) {
        return NULL;
    }

    for(uint32_t i = 0; i < 226; ++i) {
        if(0 == (table->nodes[i].flags & NODE_VALID)) {
            return &(table->nodes[i]);
        }
    }

    return NULL;
}

uint32_t mem_alloc(uint32_t size) {
    if(size > 4096) {
        /* maximum allocation size is 4k currently */
        return NULL;
    }

    /* walk allocation tables, find space that fits exactly, if none fit exactly, return the one that fits best */
    alloc_node_t* best_node = NULL;
    alloc_table_t* best_table = NULL;
    alloc_table_t* table = &base_table;

    while(table != NULL) {
        alloc_node_t* node = mem_get_best_node(table, size);
        if(node != NULL) {

            if(best_node == NULL) {
                best_node = node;
                best_table = table;
            }

            if(node->size == size) {
                best_node = node;
                best_table = table;
                break;
            }

            if(node->size < best_node->size) {
                best_node = node;
                best_table = table;
            }
        }

        table = (alloc_table_t*) table->next_table;
    }

    /* we now have the best fitting node */

    /* if it fits exactly, set the node as allocated */
    if(best_node->size == size) {
        best_node->flags |= NODE_ALLOCATED;
        return best_node->start;
    }

    /* doesn't fit exactly, split the node */
    if(best_node->size > size) {
        alloc_node_t* next_node = (alloc_node_t*) best_node->next_node;
        alloc_node_t* new_node = mem_get_unused_node(best_table);

        if(new_node == NULL) {
            /* out of nodes to use, abort */
            return NULL;
        }

        new_node->start = best_node->start + size;
        new_node->size = best_node->size - size;
        new_node->next_node = (uint32_t) next_node;
        new_node->flags = NODE_VALID;

        best_node->size -= size;
        best_node->next_node = (uint32_t) new_node;
        best_node->flags |= NODE_ALLOCATED;

        return best_node->start;
    }

    /* execution should never get here, we must have failed */
    return NULL;
}

void mem_free(uint32_t addr) {
    /* walk the tables, find the node that starts at addr */
    alloc_table_t* table = &base_table;
    alloc_node_t* node = NULL;
    alloc_node_t* prev_node = NULL;

    while(table != NULL) {
        node = (alloc_node_t*) table->start_node;

        while(node != NULL) {

            if(node->start == addr) {
                break;
            }

            prev_node = node;
            node = (alloc_node_t*) node->next_node;
        }

        if(node && node->start == addr) {
            break;
        }

        table = (alloc_table_t*) table->next_table;
    }

    if(node == NULL || node->start != addr) {
        /* No such allocation, hmmm */
        return;
    }

    /* mark the node as free */
    if(node->flags & NODE_ALLOCATED) {
        node->flags ^= NODE_ALLOCATED;
    }

    /* now lets merge any adjacent free nodes */

    alloc_node_t* next_node = (alloc_node_t*) node->next_node;

    /* check if next node is free, if so, merge into it */
    if(next_node != NULL && next_node->flags & NODE_VALID && 0 == (next_node->flags & NODE_ALLOCATED)) {
        node->size += next_node->size;
        node->next_node = next_node->next_node;
        next_node->flags = 0;
    }

    /* check if previous node is free, if so, it merges into us */
    if(prev_node != NULL && prev_node->flags & NODE_VALID && 0 == (prev_node->flags & NODE_ALLOCATED)) {
        prev_node->size += node->size;
        prev_node->next_node = node->next_node;
        node->flags = 0;
    }
}
