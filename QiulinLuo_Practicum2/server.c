#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/vfs.h>
#include "server.h"
#include <sys/mount.h>

#ifdef __APPLE__
#define st_mtim st_mtimespec
#endif

int main() {
    // Initialize the container
    memset(ser_msg, '\0', sizeof(ser_msg));
    memset(clt_msg, '\0', sizeof(clt_msg));
    memset(cmd, '\0', sizeof(cmd));
    memset(fpath, '\0', sizeof(fpath));
    memset(deliver, '\0', sizeof(deliver));
    memset(delim, '\0', sizeof(delim));

    soc = socket(AF_INET, SOCK_STREAM, 0);

    if (soc < 0) {
        perror("error creating socket");
        return -1;
    }
    printf("\nsocket created successfully\n");

    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(PORT);
    ser_addr.sin_addr.s_addr = inet_addr(HOST);

    if (bind(soc, (struct sockaddr*)&ser_addr, sizeof(ser_addr)) < 0) {
        perror("error binding");
        return -1;
    }
    printf("done with binding\n");

    if (listen(soc, 10) < 0) {
        perror("error listening");
        return -1;
    }
    
    strcpy(makedir, "mkdir ");
    strcat(makedir, "USB1");
    system(makedir);
    memset(makedir, '\0', sizeof(makedir));
    strcpy(makedir, "mkdir ");
    strcat(makedir, "USB2");
    system(makedir);

    // Start to link the USB device
    printf("please insert the first USB device: \n");

    char tmp[50];
    scanf("%s", tmp);
    strcpy(linkUSB_cmd, "sudo mount ");
    strcat(linkUSB_cmd, tmp);
    strcat(linkUSB_cmd, " ./USB1");

    if (system(linkUSB_cmd) == 0) printf("the first USB is ready\n");
    else printf("the first USB is not ready\n");

    memset(tmp, '\0', sizeof(tmp));
    memset(linkUSB_cmd2, '\0', sizeof(linkUSB_cmd));

    printf("please insert the second USB device: \n");
    scanf("%s", tmp);
    strcpy(linkUSB_cmd2, "sudo mount ");
    strcat(linkUSB_cmd2, tmp);
    strcat(linkUSB_cmd2, " ./USB2");

    if (system(linkUSB_cmd2) == 0) printf("the second USB is ready\n");
    else printf("the second USB is not ready\n");

    printf("Listening.....\n");

    while (1) {
        clt_volume = sizeof(clt_addr);
        clt_soc = accept(soc, (struct sockaddr*)&clt_addr, &clt_volume);
        if (clt_soc < 0) return -1;
        
        printf("client connected at IP: %s and port: %i\n", inet_ntoa(clt_addr.sin_addr), ntohs(clt_addr.sin_port));
        
        if (fork() == 0) {
            if (recv(clt_soc, clt_msg, sizeof(clt_msg), 0) < 0) {
                perror("error receiving");
                return -1;
            }
          
            sprintf(delim, "%c", DELIM);
            strcpy(cmd, strtok(clt_msg, delim));
            strcpy(fpath, strtok(NULL, delim));
            
            // Split the command
            if (strcmp(cmd, "GET") == 0) {
                get_func(fpath, ser_msg);
            } 
            else if (strcmp(cmd, "INFO") == 0) {
                info_func(fpath, ser_msg);
            } 
            else if (strcmp(cmd, "MD") == 0) {
                md_func(fpath, ser_msg);
            } 
            else if (strcmp(cmd, "PUT") == 0) {
                strcpy(deliver, strtok(NULL, delim));
                put_func(fpath, deliver, ser_msg);
            } 
            else if (strcmp(cmd, "RM") == 0) {
                rm_func(fpath, ser_msg);
            } 
            else {
                strcpy(ser_msg, "your command is not right\n");
            }

            if (send(clt_soc, ser_msg, strlen(ser_msg), 0) < 0) {
                perror("error sending back to client");
                return -1;
            }
            memset(ser_msg, '\0', sizeof(ser_msg));
            memset(clt_msg, '\0', sizeof(clt_msg));
            memset(cmd, '\0', sizeof(cmd));
            memset(fpath, '\0', sizeof(fpath));
            memset(deliver, '\0', sizeof(deliver));
            memset(delim, '\0', sizeof(delim));
            close(soc);
            close(clt_soc);
        } 
        else {
            close(clt_soc);
            memset(ser_msg, '\0', sizeof(ser_msg));
            memset(clt_msg, '\0', sizeof(clt_msg));
            memset(cmd, '\0', sizeof(cmd));
            memset(fpath, '\0', sizeof(fpath));
            memset(deliver, '\0', sizeof(deliver));
            memset(delim, '\0', sizeof(delim));
        }
    }
    close(soc);
    close(clt_soc);
    return 0;
}

void get_func(char const* fpath, char* ser_msg) {
    char* file_content;
    char usb_chosen_path[strlen(fpath) + strlen(USB[0]) + 10];
    int flag = 0;
    
    for(int i = 0; i < 2; i++){
        memset(usb_chosen_path, '\0', sizeof(usb_chosen_path));
    	strcat(usb_chosen_path, USB[i]);
    	strcat(usb_chosen_path, fpath);

    	FILE* tmp_file = fopen(usb_chosen_path, "rb");
    	if (tmp_file == NULL) {
            flag++;
            continue;
    	}
        else {
            fseek(tmp_file, 0, SEEK_END);
    	    int file_length = ftell(tmp_file);
    	    rewind(tmp_file);
    	    int read_length = fread(ser_msg, 1, file_length, tmp_file);
    	    if (read_length != file_length) {
                sprintf(ser_msg, "read file %s failed!\n", fpath);
                return;
    	    }
    	    fclose(tmp_file);
    	    tmp_file = NULL;
        }
    }
    if(flag == 2){
        sprintf(ser_msg, "open file %s failed!\n", fpath);
        return;
    }
}

void info_func(char const* fpath, char* ser_msg) {
    char auth[20] = {0};
    char tmp_fpath[strlen(fpath) + strlen(USB[0]) + 10];
    int flag = 0;
    
    for (int i = 0; i < 2; i++) {
    	memset(tmp_fpath, '\0', sizeof(tmp_fpath));
        strcat(tmp_fpath, USB[i]);
        strcat(tmp_fpath, fpath);
         
        FILE *tmp_file = fopen(tmp_fpath, "rb");
        if (tmp_file == NULL) {
            flag++;
            continue;
        }
        else {
            // Obtain the authorization of system
            sprintf(auth, "%s%s%s", !access(tmp_fpath, R_OK) ? "read " : "", !access(tmp_fpath, W_OK) ? "write " : "", !access(tmp_fpath, X_OK) ? "execute " : "");
            struct stat cur;
            stat(tmp_fpath, &cur);
            sprintf(ser_msg,"user id: %d\n" "group id: %d\n" "last modification: %s" "size: %ld bytes\n" "permission: %s\n", cur.st_uid, cur.st_gid, ctime(&cur.st_mtim.tv_sec), cur.st_size, auth);
        }  
    }  
    if (flag == 2) {
        sprintf(ser_msg, "file %s doesn't exist!\n", fpath);
    }
}

void put_func(char const* fpath, char* deliver, char* ser_msg) {
    char por_fpath[strlen(fpath) + strlen(USB[0]) + 10];
    int flag = 0;
    
    for (int i = 0; i < 2; i++) {
        memset(por_fpath, '\0', sizeof(por_fpath));
        strcat(por_fpath, USB[i]);
        strcat(por_fpath, fpath);
        
        FILE* tmp_file = fopen(por_fpath, "wb+");
        if (tmp_file == NULL) {
            flag++;
            continue;
        }
        else {
            int write_length = fwrite(deliver, 1, strlen(deliver), tmp_file);
            if (write_length != strlen(deliver)) {
                sprintf(ser_msg, "can't write file to %s\n", por_fpath);
                return;
            }
            fclose(tmp_file);
            tmp_file = NULL;
        } 
    }
    if (flag == 2) {
        strcpy(ser_msg, "can't open file\n");
        return;
    }
    else {
    	sprintf(ser_msg, "%s has been put\n", fpath);
    }
}

void md_func(char const* fname, char* ser_msg) {
    char por_fname[strlen(fname) + strlen(USB[0]) + 10];
    
    for (int i = 0; i < 2; i++) {
	memset(por_fname, '\0', sizeof(por_fname));
	strcat(por_fname, USB[i]);
        strcat(por_fname, fname);
        
        if (!access(por_fname, F_OK)) {
            sprintf(ser_msg + strlen(ser_msg), "%s is duplicated\n", fname);
            return;
        }
        mkdir(por_fname, 0777);
    }
    sprintf(ser_msg + strlen(ser_msg), "%s has been created\n", fname);
}

void rm_func(char const* path, char* ser_msg) {
    char rm_path[strlen(path) + strlen(USB[0]) + 10];
    int flag = 0;
    
    for (int i = 0; i < 2; i++) {
        memset(rm_path, '\0', sizeof(rm_path));
        strcat(rm_path, USB[i]);
        strcat(rm_path, path);
        
        if (access(rm_path, F_OK)) {
            flag++;
            continue;
        }
        else {
            remove(rm_path);
        }
    }
    if (flag == 2) {
        sprintf(ser_msg, "can't find file %s\n", path);
        return;
    }
    else {
        sprintf(ser_msg, "%s has been removed\n", path);
    }
    
}

