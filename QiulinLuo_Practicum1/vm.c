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

void *init() {
  pthread_mutex_lock(&vm_lock);
  for(int j = 0; j < sizeof(v_T) / sizeof(v_T[0]); j++) v_T[j] = FREED;
  for(int k = 0; k < PAGE_NUM; k++){
    p_T[k] = FREED;
    t_T[k] = FREED;
  }
  pthread_mutex_unlock(&vm_lock);
}

void *my_malloc(size_t size) {
  int i = 0;
  void *alloc = NULL;
  if(size <= 0) return NULL;
  if(size > PAG_SIZE) return NULL;
  if (calc > THRESHOLD) {
    printf("Excute a swap operations.\n");
    swapHandler(phys_mem + 0 * PAG_SIZE, t_T[0], p_T[0]);
  } 
  while (i < VM_NUM) {
    if(calc > THRESHOLD) break;
    if (v_T[i] == FREED) {
      v_T[i] = calc;
      t_T[calc] = i;
      p_T[calc] = size;
      calc = calc + 1;
      alloc = phys_mem + PAG_SIZE * i;
      break;     
    }   
    i = i + 1;
  }
  if (i == VM_NUM) return NULL;
  for (int i = 0; i < VM_MAX_LONG ; i++) printf("vm malloc table (No.%d Val:%d)\n", i, v_T[i]);
  for (int k = 0; k < PAGE_NUM ; k++) printf("phsical malloc table (No.%d Val:%d)\n", k, p_T[k]);
  return alloc;
}

void* renew_t_p_T(int id, int *arr) {
  for (int j = id; j < PAGE_NUM - 1; j++) {
      int exh = arr[j];
      arr[j] = arr[j + 1];
      arr[j + 1] = exh;
    }
}

void* renew_v_T() {
  for (int j = 0; j < PAGE_NUM; j++) {
    if (t_T[j] == FREED) continue;
    v_T[t_T[j]] = j;
  }
}

void* renew_p_T() {
  for(int r = 0; r < PAGE_NUM; r++) {
    if (t_T[r] == FREED) p_T[r] = FREED;
  }
}

void* renew_calc() {
  int renewl = 0;
  for(int q = 0; q < PAGE_NUM; q++) {
    if (p_T[q] >= 0){
      renewl = renewl + 1;
    }
    calc = renewl;
  }
}

void* my_free(void *ptr) {
  if (calc >= 0){
    if(v_T[(ptr - (void*)phys_mem) / PAG_SIZE] == OUT) {
      printf("Page %d's location is swapfile, wait for operations.\n", (ptr - (void*)phys_mem) / PAG_SIZE);
      for(int i = 0; i < PAGE_NUM; i++) {
        if (v_T[i + VM_NUM] == (ptr - (void*)phys_mem) / PAG_SIZE) {
          rmFile(v_T[i + VM_NUM]);
          v_T[i + VM_NUM] = FREED;
          v_T[(ptr - (void*)phys_mem) / PAG_SIZE] = FREED;
        }
      }
    }
    if(v_T[(ptr - (void*)phys_mem) / PAG_SIZE] >= 0) {
      printf("Page %d's location is physical heap, wait for operations.\n", (ptr - (void*)phys_mem) / PAG_SIZE);
      int tmp = v_T[(ptr - (void*)phys_mem) / PAG_SIZE];
      v_T[(ptr - (void*)phys_mem) / PAG_SIZE] = FREED;
      t_T[tmp] = FREED;
      renew_t_p_T(tmp, t_T);
      renew_v_T();
    }
    renew_p_T(0, p_T);
    renew_calc();
  }   
  for (int i = 0; i < VM_MAX_LONG ; i++) printf("vm free table (No.%d Val:%d)\n", i, v_T[i]);
  for (int k = 0; k < PAGE_NUM ; k++) printf("phsical free table (No.%d Val:%d)\n", k, p_T[k]);
}

void* swapHandler(void *ptr, int idSwap, int val) {
  if (calc >= 0) {
    for (int j = 0; j < PAGE_NUM; j++) {
        if (v_T[j + VM_NUM] < 0) {
          v_T[j + VM_NUM] = idSwap;
          resvArr(idSwap, val);
          break;
        }
    }
    v_T[idSwap] = OUT;
    t_T[0] = FREED;
    renew_t_p_T(0, t_T);
    renew_v_T();
    p_T[0] = FREED;
    renew_t_p_T(0, p_T);
    renew_calc();
  }
}

void *thread_func_malloc(void *arg) {    
    pthread_mutex_lock(&vm_lock);
    size_t size = (size_t) arg;
    void *ptr = my_malloc(size);
    pthread_mutex_unlock(&vm_lock);
    return ptr;
}

void *thread_func_free(void *arg) {    
    pthread_mutex_lock(&vm_lock);
    my_free(arg);
    pthread_mutex_unlock(&vm_lock);
}

void *resvArr(int id, int val) {
    FILE *fp;
    char fid[100];
    sprintf(fid, "%d.bin", id);
    fp = fopen(fid, "w");
    if (fp == NULL) exit(EXIT_FAILURE);
    fprintf(fp, "%d", val);
    fclose(fp);
}

int rmFile(int id) {
    char fid[100];
    sprintf(fid, "%d.bin", id);
    int enq = remove(fid);
    if (enq == 0) return 1;
    else return 0;
}

int getMemoryInfo(int id) {
    FILE *fp;
    char fid[100];
    sprintf(fid, "%d.bin", id);
    fp = fopen(fid, "r");
    if (fp == NULL) exit(EXIT_FAILURE);
    int val;
    fscanf(fp, "%d", &val);
    fclose(fp);
    return val;
}

void* operateFile(int digit) {
  pthread_mutex_lock(&vm_lock);
  int enq;
  if (v_T[digit] == FREED) {
    enq = FREED;
    printf("Freed page %d.\n", enq);
  } else if (v_T[digit] == OUT){
    for (int i = 0; i< PAGE_NUM; i++) {
      if (v_T[i + VM_NUM] == digit) {
        if (calc <= THRESHOLD){
          enq = getMemoryInfo(digit);
          printf("Swapfile Memory page %d enters %d.\n", digit, enq);
          rmFile(digit);
          v_T[i + VM_NUM] = FREED;
          renew_calc();
          v_T[digit] = calc;
          t_T[calc] = digit;
          p_T[calc] = enq;
          renew_v_T();
        } else {
          printf("Physical memory deny to join.\n");
        }
      }
    }
    renew_calc();
  } 
  else {
    int id = v_T[digit];
    v_T[digit] = FREED;
    enq = p_T[id];
    for(int i = id; i < PAGE_NUM - 1; i++) {
      if(p_T[i + 1] == FREED) break;
      p_T[i] = p_T[i + 1];
      p_T[i + 1] = enq;
    }
    for(int i = id; i < PAGE_NUM - 1; i++) {
      if(t_T[i + 1] == FREED) break;
      t_T[i] = t_T[i + 1];
      t_T[i + 1] = enq;
    }
    renew_v_T();
    renew_calc();
    printf("Physical memory page %d enters %d.\n", digit, enq);
  }
  for (int i = 0; i < VM_MAX_LONG ; i++) printf("vm final table (No.%d Val:%d)\n", i, v_T[i]);
  for (int k = 0; k < PAGE_NUM ; k++) printf("phsical final table (No.%d Val:%d)\n", k, p_T[k]);
  pthread_mutex_unlock(&vm_lock);
}
