#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "netinet/in.h"

#define BUF_SIZE 1024 
#define MSG_SIZE 256	//클라이언트 -> 서버에게 보내는 쿼리문 길이 

int init_socket();
void send_query(int connect_fd);
void close_socket(int connect_fd);
