#include "c.h"

void init_socket(){
	struct sockaddr_in connectSocket;
	memset(&connectSocket, 0, sizeof(connectSocket));
	connectSocket.sin_family = AF_INET;
	inet_aton("172.24.149.220", (struct in_addr*) &connectSocket.sin_addr.s_addr);
	connectSocket.sin_port = htons(12345);

	printf(" .. : %s \n",strerror(errno));
	int connect_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(connect(connect_fd, (struct sockaddr*) &connectSocket, sizeof(connectSocket))<0){
		printf("Socket Error .. : %s \n",strerror(errno));
        	exit(1);
	}
	else{
		int readBytes, writtenBytes;
		char sendBuff[BUF_SIZE], readBuff[BUF_SIZE];
		memset(sendBuff, 0, sizeof(sendBuff));
	
		while(1){
			//서버에 문자열을 보낸 뒤 서버가 보낸 echo를 받아 출력.
			printf("서버에 보낼 말을 입력하세요 :: ");
			fgets(sendBuff,MSG_SIZE,stdin); //키보드 입력
			if(strcmp(sendBuff,"dd\n")==0){
				break;
			}
			write(connect_fd, sendBuff, strlen(sendBuff)); //서버에 보내기 (쓰기)
			if((readBytes = read(connect_fd, readBuff, BUF_SIZE))<=0)//서버에서 보낸 값을 읽기
				printf("Read Error... : %s \n",strerror(errno));
			printf("readBuff : %s\n",readBuff);
			printf("%d bytes read\n",readBytes);
			readBuff[readBytes] = '\0';
			fputs(readBuff,stdout); //서버 응답을 화면에 출력
			fflush(stdout);	//출력버퍼 비워주기
		}
	}
	close(connect_fd);
	exit(0);
}

int main(){
	init_socket();
	return 0;
}
