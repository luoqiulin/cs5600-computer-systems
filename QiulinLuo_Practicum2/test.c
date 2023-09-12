#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char cmd[100] = "GET folder1/hi.txt\n";
    char cmd2[100] = "GET folder1/hi.txt no.txt\n";
    char cmd3[100] = "PUT folder1/hi.txt hi.txt\n";
    char cmd4[100] = "PUT hi.txt\n";
    char cmd5[100] = "INFO folder1/hi.txt\n";
    char cmd6[100] = "MD newfolder\n";
    char cmd7[100] = "RM folder1/hii.txt\n";
    
    FILE* file = popen("./client", "w");
    if (file == NULL) {
        perror("popen failed");
        return EXIT_FAILURE;
    }
    fputs(cmd, file);
    fputs(cmd2,file);
    fputs(cmd3, file);
    fputs(cmd4,file);
    fputs(cmd5,file);
    fputs(cmd6,file);
    fputs(cmd7,file);
    
    pclose(file);
    exit(0);
    return EXIT_SUCCESS;
}

