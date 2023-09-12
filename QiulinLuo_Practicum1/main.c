#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include "vm.h"

pthread_t initialization;
pthread_t test1[VM_NUM];
pthread_t test2[VM_NUM];
pthread_t test3;
void* record[PAGE_NUM];

int main() {
  pthread_mutex_init(&vm_lock, NULL);
  pthread_create(&initialization, NULL, init, NULL);
  pthread_join(initialization, NULL);
  
  // Malloc test.
  for (int i = 0; i < VM_NUM; i++) pthread_create(&test1[i], NULL, thread_func_malloc, i + 1);
  for (int i = 0; i < VM_NUM; i++) pthread_join(test1[i], &record[i]);

  printf("Test 1 Done.\n\n\n");

//  // Free test.
//  for (int i = 1; i < 4; i++) pthread_create(&test2[i], NULL, thread_func_free, record[i]);
//  for (int i = 1; i < 4; i++) pthread_join(test2[i], NULL);

//  printf("Test 2 Done.\n\n\n");

//  // Operate swapfile test.
//  pthread_create(&test3, NULL, operateFile, 5);
//  pthread_join(test3, NULL);
  pthread_mutex_destroy(&vm_lock);
  printf("Test 3 Done.\n\n\n");

  return 0;
}
