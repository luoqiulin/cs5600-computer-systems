#include "pmHeap.h"
#include <stdio.h>

currentHeap customHeap;
void* memBegin;
void* memEnd;

void initialization(void* start, size_t size){
    memBegin = start;
    memEnd = memBegin + size;

    customHeap.used = 0;
    customHeap.num_alloc = 0;

    currentHeader* tmp = (currentHeader*)memBegin;
    tmp->sig = DEFAULT_LOWER_BOUND;
    tmp->note = 0;
    tmp->prev = 0;
    tmp->size = size - sizeof(currentHeader);
}

void* pm_malloc(size_t size){
    void* current = memBegin;
    while(current < memEnd){
        currentHeader* block = (currentHeader*)current;

        if(block->note == 0 && block->size >= size){
            size_t previous_size = block->size;

            block->note = 1;
            block->size = size;

            int next_size = previous_size - block->size;

            if(next_size < sizeof(currentHeader)){
                int lost_bytes = next_size;
                block->size += lost_bytes;
            }
            else{
                currentHeader* next = (currentHeader*)(current + block->size + sizeof(currentHeader));
                next->sig = DEFAULT_LOWER_BOUND;
                next->note = 0;
                next->prev = current;
                next->size = next_size - sizeof(currentHeader);
            }

            customHeap.used += block->size;
            customHeap.num_alloc++;
            return (void*)(current + sizeof(currentHeader));
        }
        else{
            current += (sizeof(currentHeader) + block->size);
        }
    }

    return 0;
}

int block_judgement(void* current){
    if(current < memBegin || current >= memEnd) return 0;

    currentHeader* block = (currentHeader*)current;

    if(block->sig != DEFAULT_LOWER_BOUND) return 0;

    if(block->note == 1) return 0;
    else return 1;
}

void pm_free(void* ptr){
    currentHeader* block = (currentHeader*)(ptr - sizeof(currentHeader));
    block->note = 0;

    customHeap.num_alloc--;
    customHeap.used -= block->size;

    currentHeader* next = (currentHeader*)(ptr + block->size);
    if(block_judgement(next)){
        next->sig = DEFAULT_UPPER_BOUND;

        size_t total_size = (next->size + sizeof(currentHeader));
        block->size += total_size;
    }

    currentHeader* prev = (currentHeader*)(block->prev);
    if(block_judgement(prev)){
        block->sig = DEFAULT_UPPER_BOUND;

        size_t total_size = (block->size + sizeof(currentHeader)) + (prev->size);
        prev->size = total_size;
        next->prev = (void*)prev;
    }
}

int block_calculation(){
    int blocks = 0;

    void* current = memBegin;
    while(current < memEnd){
        currentHeader* block = (currentHeader*)current;
        printf("block %p, size: %ld, start pointer(%s): %p\n", current, block->size, block->note == 0 ? "free" : "used", block->prev);
        blocks++;
        current += (block->size + sizeof(currentHeader));
    }

    return blocks;
}

currentHeap heap_inquiry(){
    return customHeap;
}
