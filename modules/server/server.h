#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <libpq-fe.h>

#include "netinet/in.h"

#define MSG_SIZE 256	//클라이언트 -> 서버로 읽어들이는 값
#define BUF_SIZE 4096

//전역변수로 선언하지 않고.. main에서 ? 

struct sockaddr_in server_addr, client_addr;
char client_ip[20];
int listen_fd, connect_fd;
int client_len;

struct Result;

void init_socket();
void listen_socket();
void connect_socket();
void close_socket(PGconn*);
void send_result(int, struct Result*, char*, char*);

