#ifndef _SERVERFUNC_H
#define _SERVERFUNC_H

#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

#define DELIM 0x20

// Set the maximum capacity of message and command container.
#define CAP 300000000

// Deploy the basic parameter of socket connection.
const char* HOST = "127.0.0.1";
const int PORT = 2000;
char *USB[] = {"USB1/", "USB2/"};
int soc;
int clt_soc;
socklen_t clt_volume;

// Set the message or command container.
struct sockaddr_in ser_addr, clt_addr;
char linkUSB_cmd[100];
char linkUSB_cmd2[100];
char makedir[100];
char ser_msg[CAP];
char clt_msg[CAP];
char cmd[5];
char fpath[100];
char deliver[1024];
char delim[2];

extern void get_func(const char* fpath, char* ser_msg);
extern void info_func(char const* fpath, char* ser_msg);
extern void put_func(char const* fpath, char* deliver, char* ser_msg);
extern void md_func(char const* fname, char* ser_msg);
extern void rm_func(char const* path, char* ser_msg);

#endif
