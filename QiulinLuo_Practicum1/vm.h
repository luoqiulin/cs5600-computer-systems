#ifndef _VM_H
#define _VM_H

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>

#define PHY_SIZE 10 * 1024 * 1024
#define PAGE_NUM 10
#define PAG_SIZE (PHY_SIZE / PAGE_NUM)
#define VM_NUM (PAGE_NUM + PAGE_NUM)
#define THRESHOLD (PAGE_NUM - 1)
#define VM_MAX_LONG (VM_NUM + PAGE_NUM)
#define FREED -1
#define OUT -2

static pthread_mutex_t vm_lock;
static char phys_mem[PHY_SIZE];
static int p_T[PAGE_NUM];
static int v_T[VM_MAX_LONG];
static int t_T[PAGE_NUM];
static int calc;

extern void *thread_func_malloc(void *arg);
extern void *thread_func_free(void *arg);
extern void *init();
extern void* renew_v_T();
extern void* renew_p_T();
extern void* renew_calc();
extern void* renew_t_p_T(int id, int *arr);
extern void *my_malloc(size_t size);
extern void *my_free(void *ptr);
extern void *swapHandler(void *ptr,int idSwap,int val);
extern void *resvArr(int id, int val);
extern int rmFile(int id);
extern int getMemoryInfo(int id);
extern void* operateFile(int digit);

#endif
