#ifndef _PMHEAP_H
#define _PMHEAP_H

#include <stdio.h>
#include <stdint.h>

#define DEFAULT_LOWER_BOUND 0xAC63F59E
#define DEFAULT_UPPER_BOUND 0xE30E0150

typedef struct{
    uint32_t used;
    uint32_t num_alloc;
} currentHeap;

typedef struct{
    uint32_t sig;
    uint8_t note;
    void* prev;
    size_t size;
} currentHeader;

extern void initialization(void* start, size_t size);
extern int block_calculation();
extern void* pm_malloc(size_t size);
extern void pm_free(void* ptr);
extern currentHeap heap_inquiry();

#endif
