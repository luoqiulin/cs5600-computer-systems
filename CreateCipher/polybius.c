#include <stdio.h>
#include <string.h>
#include "polybius.h"
#include <ctype.h>

// Returns special characters directly.

void pbDecode(const char *ciphertext, table t){
    for(int i = 0; i < strlen(ciphertext); i++){
        if(ciphertext[i] - '0' >= 0 && ciphertext[i] - '9' <= 0){
            int marker = -1;
            int dash = ciphertext[i] - '0';
            int dot = ciphertext[++i] - '0';
            for(int j = 0; j < 26; j++) {
                if(t.conversion[j][0] == dash && t.conversion[j][1] == dot){
                    marker = j;
                    break;
                }
            }
            if(marker != -1) printf("%c", toupper(t.alphabet[marker]));
            else printf(" "); 
        }
        else printf("%c", ciphertext[i]); 
    }
    printf("\n");
}

void pbEncode(const char *plaintext, table t){
    for(int i = 0; i < strlen(plaintext); i++){
        if(isalpha(plaintext[i])){
            printf("%d%d ", t.conversion[toupper(plaintext[i]) - 'A'][0], t.conversion[toupper(plaintext[i]) - 'A'][1]);
        }
        else printf("%c", plaintext[i]); 
    }
    printf("\n");
}


