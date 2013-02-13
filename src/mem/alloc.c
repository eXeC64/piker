#include "kernel.h"

#include "mem/alloc.h"

/*
 * alloc_table is 4kb and manages up to
 * 16 frames (64kb), and up to 335 gaps
 * within those 16 frames.
 *
 * It contains a list of frames it's using
 * and a linked list of gaps.
 *
 * It also has a pointer to the next
 * allocation table, so more than 64kb
 * can be indexed.
 */

/* 12 bytes */
typedef struct {
    uint32_t start; /* 0 if node unused, virt address (0xC0000000+) otherwise */
    uint32_t size; /* 0 if node unused, 1+ otherwise */
    struct alloc_node_t* next; /* 0 if no node after this, virt address otherwise */
} alloc_node_t;

/* 4096 bytes exactly - 1 frame */
typedef struct {
    uint32_t num_frames; /* number of frames used */
    uint32_t num_nodes; /* number of nodes used */
    struct alloc_table_t* next; /* next allocation table */
    uint32_t frames[16]; /* array of frames used */
    alloc_node_t nodes[335]; /* array of nodes in the linked list */
} alloc_table_t;

/* Base allocation table is statically declared, and page aligned */
alloc_table_t base_table __attribute__((aligned (4096)));

void* mem_alloc(uint32_t size) {
    return NULL;
}

void mem_free(void* address, uint32_t size) {

}
