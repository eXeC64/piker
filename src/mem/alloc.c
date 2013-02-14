#include "kernel.h"

#include "mem/alloc.h"
#include "mem/frame.h"

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
    alloc_node_t* first_node; /* first usable node */
    struct alloc_table_t * next; /* next allocation table */
    uint32_t frames[16]; /* array of frames used */
    alloc_node_t nodes[335]; /* array of nodes in the linked list */
} alloc_table_t;

/* Base allocation table is statically declared, and page aligned */
alloc_table_t base_table __attribute__((aligned (4096)));

void alloc_init() {
    base_table.num_frames = 0;
    base_table.first_node = NULL;
    base_table.next = NULL;

    for(uint32_t i = 0; i < 16; ++i) {
        base_table.frames[i] = 0;
    }

    for(uint32_t i = 0; i < 335; ++i) {
        base_table.nodes[i].start = 0;
        base_table.nodes[i].size = 0;
        base_table.nodes[i].next = 0;
    }

    /* We'll start with 1 frame by default */
    if( 0 == frame_alloc(base_table.frames) ) {
        return; /* If this failed we're screwed. */
    }

    base_table.num_frames = 1;

    base_table.first_node = (alloc_node_t*) &(base_table.nodes[0]);

    /* start address of free space as a virt address */
    base_table.first_node->start = base_table.frames[0] + 0xC0000000;
    base_table.first_node->size = 4096;
    base_table.first_node->next = NULL;
}

uint32_t mem_alloc(uint32_t size) {

    if(base_table.first_node == NULL && base_table.next == NULL) {
        /* No free space left. TODO allocate more space */
        return NULL;
    }

    /* search for smallest free space that fits */
    uint32_t best_size = base_table.first_node->size;
    alloc_node_t* best_node = base_table.first_node;

    alloc_node_t* prev = NULL;
    alloc_node_t* cur = base_table.first_node;
    alloc_node_t* next = (alloc_node_t*) cur->next;

    /*
       Wait until we've either found a perfect fit
       or have run out of nodes to scan
    */

    while(best_size != size) {
        if(cur->size < best_size && cur->size >= size) {
            /* New best fit */
            best_size = cur->size;
            best_node = cur;
        }
        /* Move onto the next node if valid*/
        if(next != NULL) {
        prev = cur;
        cur = next;
        } else {
            break;
        }
    }

    if(best_size == size) {
        /* Fits perfectly, this node simply gets removed */

        /* If this is the first node, point first node to next node */
        if(base_table.first_node == cur) {
            base_table.first_node = (alloc_node_t*) cur->next;
        } else {
            /* Point previous node to next node */
            if(prev != NULL) {
                prev->next = (struct alloc_node_t*) next;
            }
        }

        /* Zero out this node and return the pointer */
        uint32_t pointer = cur->start;

        cur->start = 0;
        cur->size = 0;
        cur->next = NULL;

        return pointer;

    } else {
        /* We need to shrink this node
         * It is fairly easy, we just
         * move the start of this node
         * forwards and decrease the size.
         */


        uint32_t pointer = (cur->start);

        cur->start += size;
        cur->size -= size;

        return pointer;
    }

    /* Execution should never reach here. */
    return NULL;
}

void mem_free(uint32_t address, uint32_t size) {

}
