#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include "client.h"

int main() {
    while (1) {
        // Initialize the container.
        memset(ser_msg, '\0', sizeof(ser_msg));
        memset(clt_msg, '\0', sizeof(clt_msg));
        memset(cmd, '\0', sizeof(cmd));

        soc = socket(AF_INET, SOCK_STREAM, 0);

        if (soc < 0) {
            perror("error creating socket");
            return -1;
        }

        ser_addr.sin_family = AF_INET;
        ser_addr.sin_port = htons(PORT);
        ser_addr.sin_addr.s_addr = inet_addr(HOST);

        if (connect(soc, (struct sockaddr*)&ser_addr, sizeof(ser_addr)) < 0) {
            perror("error connecting to socket");
            return -1;
        }
	
        // If connect successfully, then start to input commands.
        char arg1[500];
        char arg2[500];
        char arg3[500];
        memset(arg1, '\0', sizeof(arg1));
        memset(arg2, '\0', sizeof(arg2));
        memset(arg3, '\0', sizeof(arg3));
        memset(static_token, '\0', sizeof(static_token));
        printf("\ninput command: ");
        gets(cmd);
        sprintf(clt_msg, "%s", cmd);
        token = strtok(cmd, " ");
        sprintf(static_token, token);
        int i = 0;
       
        while (token != NULL) {
            if (i == 0) sprintf(arg1, token);
            if (i == 1) sprintf(arg2, token);
            if (i == 2) sprintf(arg3, token);
            i++;
            token = strtok(NULL, " ");
        }
        
        // Split command.
        if (static_token[0] == 'P' && i == 3) {
            memset(clt_msg, '\0', sizeof(clt_msg));
            FILE *p = fopen(arg3, "rb+");
            char content[1024];
            char container[1024];
            memset(content, '\0', sizeof(content));
            memset(container, '\0', sizeof(container));
            
            if (p == NULL) {
                perror("can't open file");
            }
            fseek(p, 0, SEEK_END);
    	    int file_length = ftell(p);
    	    rewind(p);
    	    int read_length = fread(content, 1, file_length, p);
    	    if (read_length != file_length) {
                perror("can't read file");
    	    }
    	    fclose(p);
    	    strcpy(container, arg1);
            strcat(container, " ");
            strcat(container, arg2);
            strcat(container, " ");
            strcat(container, content);
            sprintf(clt_msg, container);
        }
        else if (static_token[0] == 'P' && i == 2) {
            memset(clt_msg, '\0', sizeof(clt_msg));
            FILE *p = fopen(arg2, "rb+");
            char content[1000];
            char container[10000];
            memset(content, '\0', sizeof(content));
            memset(container, '\0', sizeof(container));
            
            if (p == NULL) {
                perror("can't open file");
            }
            fseek(p, 0, SEEK_END);
    	    int file_length = ftell(p);
    	    rewind(p);
    	    int read_length = fread(content, 1, file_length, p);
    	    if (read_length != file_length) {
                perror("can't read file");
    	    }
    	    fclose(p);
    	    strcpy(container, arg1);
            strcat(container, " ");
            strcat(container, arg2);
            strcat(container, " ");
            strcat(container, content);
            sprintf(clt_msg, container);
        }
        
        if (send(soc, clt_msg, strlen(clt_msg), 0) < 0) {
            perror("error sending message");
            return -1;
        }
        if (recv(soc, ser_msg, sizeof(ser_msg), 0) < 0) {
            perror("error receiving message");
            return -1;
        }
        else {
            if (static_token[0] == 'G' && i == 3) {
                FILE *p = fopen(arg3, "wb+");
                if (p == NULL) {
                    perror("can't open file");
                }
                fputs(ser_msg, p);
                fclose(p);
                printf("response from server: file has been transfered\n");
            }
            else if (static_token[0] == 'G' && i == 2) {
                char *filename = strrchr(arg2, '/') + 1;
                FILE *p = fopen(filename, "wb+");
                if (p == NULL) {
                    perror("can't open file");
                }
                fputs(ser_msg, p);
                fclose(p);
                printf("response from server: file has been transfered\n");
            }
            else {
                printf("response from server: %s", ser_msg);
            }
        }
        close(soc);
    }

    close(soc);
    return 0;
}
