#include <stdio.h>
#include <string.h>
#include "polybius.h"
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

char wholetext[40][300] = {'\0'}; // Use a 2-dimension array to store sentences.

int main(){
    table t = {{{1,1},{1,2},{1,3},{1,4},{1,5},
             {2,1},{2,2},{2,3},{2,4},{2,5},
             {3,1},{3,2},{3,3},{3,4},{3,5},
             {4,1},{4,2},{4,3},{4,4},{4,5},
             {5,1},{5,2},{5,3},{5,4},{5,5}},
             {'a','b','c','d','e','f','g','h',
             'i','j','k','l','m','n','o','p',
             'q','r','s','t','u','v','w','x',
             'y','z'}};
    
    char *filename = "Arknights.txt";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error!\n");
        return 1;
    }

    char ch;
    int i = 0, j = 0;
    int flag = 0;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '.') flag = 1;
        if (ch == ' ' && flag == 1) {
            wholetext[i][j] = '\n';
            i++;
            j = 0;
            flag = 0;
            continue;
        }
        wholetext[i][j] = ch;
        j++;
    }
    fclose(fp);
    
    // Loop through the sentences and call the cipher program.
    for (int p = 0; p < 40; p++) {
        if (wholetext[p][0] != '\0') {
            char *sentence = "sentence";
            char id[5];
            sprintf(id, "%d", p);
            char *whole = (char *) malloc(strlen(sentence) + strlen(id));
            strcpy(whole, sentence);
            strcat(whole, id);
            pbEncodeLoop(wholetext[p], whole, t);
        }
    }
    
    //Modify the above loop so that the encryptions are done concurrently by spawning a separate thread for the encryption of each sentence.
    pthread_t tids[40];
    struct thread_data td[40];
    
    printf("The results have been exported to the LoopResult and ThreadsResult folders.\n");
    
    for (int i = 0; i < 40; ++i){
        if(wholetext[i][0] != '\0'){
            char *sentence2 = "sentence";
            char id2[5];
            sprintf(id2, "%d", i);
            char *whole = (char *) malloc(strlen(sentence2) + strlen(id2));
            strcpy(whole, sentence2);
            strcat(whole, id2);
            td[i].plaintext = wholetext[i];
            td[i].filename = whole;
            td[i].t = t;
            int ret = pthread_create(&tids[i], NULL, pbEncodeThreads, (void *)&td[i]);
            if (ret != 0) printf("Error!");
        }
    }
    pthread_exit(NULL);
    return 0;
}
