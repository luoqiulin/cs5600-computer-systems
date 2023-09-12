#include <stdio.h>
#include <string.h>
#include "polybius.h"
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void pbEncodeLoop(const char *plaintext, char *filename, table t){
    FILE *fp;
    char *faddress = "./LoopResult/";
    char *laddress = ".txt";
    char *tmp = (char *) malloc(strlen(faddress) + strlen(laddress) + strlen(filename));
    strcpy(tmp, faddress);
    strcat(tmp, filename);
    strcat(tmp, laddress);
    fp = fopen(tmp, "w+");
    
    for(int i = 0; i < strlen(plaintext); i++){
        if(isalpha(plaintext[i])){
            fprintf(fp, "%d%d", t.conversion[toupper(plaintext[i]) - 'A'][0], t.conversion[toupper(plaintext[i]) - 'A'][1]);
        }
        else {
            fprintf(fp, "%c", plaintext[i]);
        }
    }
}

void* pbEncodeThreads(void *threadarg){
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    
    FILE *fp2;
    char *faddress2 = "./ThreadsResult/";
    char *laddress2 = ".txt";
    char *tmp2 = (char *) malloc(strlen(faddress2) + strlen(laddress2) + strlen(my_data -> filename));
    strcpy(tmp2, faddress2);
    strcat(tmp2, my_data -> filename);
    strcat(tmp2, laddress2);
    fp2 = fopen(tmp2, "w+");
    
    for(int i = 0; i < strlen(my_data -> plaintext); i++){
        if(isalpha(my_data -> plaintext[i])){
            fprintf(fp2, "%d%d", (my_data -> t).conversion[toupper((my_data -> plaintext)[i]) - 'A'][0], (my_data -> t).conversion[toupper((my_data -> plaintext)[i]) - 'A'][1]);
        }
        else {
            fprintf(fp2, "%c", (my_data -> plaintext)[i]);
        }
    }
    pthread_exit(NULL);
}


