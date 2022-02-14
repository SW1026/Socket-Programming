#include "sys/types.h"
#include "sys/socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netinet/in.h"

#define BUF_SIZE 1024 
#define MSG_SIZE 256

void init_socket();
