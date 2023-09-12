#include <stdio.h>
#include "pmHeap.h"

#define heap_main_size 1024 * 1024

int main(){
    const int size = heap_main_size + (3 * sizeof(currentHeader));
    char custom_heap[size];
    initialization((void*)custom_heap, size);

    currentHeap state;
    void* block_1 = pm_malloc(1000);
    void* block_2 = pm_malloc(10000);
    void* block_3 = pm_malloc(100000);

    printf("In my example, I allocated 3 blocks with sizes of 1000, 10000 and 100000 respectively.\n");
    printf("\n");
    printf("Starting to allocate...\n");
    state = heap_inquiry();
    printf("already used: %d\n", state.used);
    printf("number of allocations: %d\n", state.num_alloc);
    printf("number of blocks: %d\n", block_calculation());

    pm_free(block_1);
    pm_free(block_2);
    pm_free(block_3);

    printf("\nStarting to free...\n");
    state = heap_inquiry();
    printf("already used: %d\n", state.used);
    printf("number of allocations: %d\n", state.num_alloc);
    printf("number of blocks: %d\n", block_calculation());

    return 0;
}
