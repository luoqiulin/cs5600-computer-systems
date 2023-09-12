#ifndef _CLIENT_H
#define _CLIENT_H

#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>

#define DELIM 0x20

// Set the maximum capacity of message and command container.
#define CAP 300000000

// Deploy the basic parameter of socket connection.
const char* HOST = "127.0.0.1";
const int PORT = 2000;
int soc;
char *token;
char static_token[1024];
struct sockaddr_in ser_addr; 

// Set the message or command container.
char ser_msg[CAP]; 
char clt_msg[CAP]; 
char cmd[CAP]; 
char deliver[1024];

#endif
