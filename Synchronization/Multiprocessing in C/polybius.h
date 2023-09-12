#ifndef POLYBIUS_H
#define POLYBIUS_H

typedef struct{
    int conversion[26][2];
    char alphabet[26];
} table;

struct thread_data{
    const char* plaintext;
    char* filename;
    table t;
};

extern void pbEncodeLoop(const char *plaintext, char *filename, table);
extern void* pbEncodeThreads(void* threadargs);

#endif
