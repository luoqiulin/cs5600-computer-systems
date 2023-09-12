#ifndef POLYBIUS_H
#define POLYBIUS_H

typedef struct{
    int conversion[26][2];
    char alphabet[26];
} table;

extern void pbEncode(const char *plaintext, table);
extern void pbDecode(const char *ciphertext, table);

#endif
