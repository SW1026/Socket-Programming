#include "c.h"
#include "par.h"
// g.h : send_query 의 parser() 함수

int 
init_socket()
{
	struct sockaddr_in connectSocket;
	int connect_fd;
	memset(&connectSocket, 0, sizeof(connectSocket));
	connectSocket.sin_family = AF_INET;
	inet_aton("172.24.149.220", (struct in_addr*) &connectSocket.sin_addr.s_addr);
	connectSocket.sin_port = htons(12345);
	if((connect_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("%s : Client not make socket.\n",strerror(errno));
		exit(1);
	}
	
	if(connect(connect_fd, (struct sockaddr*) &connectSocket, sizeof(connectSocket)) < 0)
	{
        printf("%s : Server not connect.\n",strerror(errno));
        exit(1);
	}
	else
	{
		return connect_fd;
	}
}

/* 통신의 지연 문제 때문에. 함수를 사용하지 않는다.
void show_result(int connect_fd, char* sendBuff, char* recvBuff, char* recv_len){
	while((recv(connect_fd, recv_len, 4, 0))>0){	//send_len 받아오는거
		if(strcmp(recv_len, "NO")==0){
			printf("Query Result>> Empty Result.\n");
			break;
		}
		if(strcmp(recv_len,"END")==0)
			break;
		if(strcmp(recv_len,"ERR")==0){		//받아오는 recv_len 크기가 4인데 버퍼오버플로우로 받아와서, show 부분까지 읽혀버림/ 왜냐면 FAIL은 5바이트여서
			printf("Server : Query Failed.\n");
			break;
		}
		////////////////////// 데이터 보내도 된다는 응답
		memset(sendBuff, 0, sizeof(sendBuff));
		sprintf(sendBuff,"OK");
		send(connect_fd, sendBuff,2,0);
		////////////////////// 데이터 하나 받아오기(쿼리결과 하나)
		memset(recvBuff, 0, sizeof(recvBuff));
		recv(connect_fd, recvBuff, atoi(recv_len),0);
		printf("%s",recvBuff);
	}
}
*/


void 
send_query(int connect_fd)
{
	char sendBuff[BUF_SIZE], recvBuff[BUF_SIZE], str[MSG_SIZE], recv_len[4];
	while(1)
	{
		memset(sendBuff, 0, sizeof(sendBuff));
		memset(recvBuff, 0, sizeof(recvBuff));
		printf("서버에 보낼 Query를 입력하세요: ");
		fgets(str,MSG_SIZE,stdin);
		char * query = parser(str);
		strcpy(sendBuff,query);

		//서버에 쿼리 날리기
		if(write(connect_fd, sendBuff, strlen(sendBuff))<0)
		{
			printf("Client : %s\n", strerror(errno));
		}
		if(strcmp(query,"BYE")==0)
		{
			return;//서버에게 종료메시지(BYE)를  보낸 후, 함수 리턴.
		}

		// ---------Server Reply---------
		if(recv(connect_fd, recvBuff, 51, 0)<0)
		{
			printf("Socket Server : %s\n",strerror(errno));
		}
		printf("%s",recvBuff);
		memset(recvBuff, 0, sizeof(recvBuff));
		memset(recv_len,0,sizeof(recv_len));
		char * dml = strtok(str," ");

		if(strcmp(dml,"show")==0)
		{	// 쿼리 결과받아오기
			while(1)
			{
				if((recv(connect_fd, recv_len, 4, 0))>=0)
				{	// send_len 받아오는거
					if(strcmp(recv_len, "NO")==0)
					{
						printf("Query Result>> Empty Result.\n");
						break;
					}
					if(strcmp(recv_len,"END")==0)
						break;
					if(strcmp(recv_len,"ERR")==0)
					{	
						printf("DB Server : Query Failed.\n");
						break;
					}
					// 데이터 보내도 된다는 응답
					memset(sendBuff, 0, sizeof(sendBuff));
					sprintf(sendBuff,"OK");
					if(send(connect_fd, sendBuff,2,0)<0)
					{
						printf("Client : %s\n",strerror(errno));
					}
					// 데이터 하나 받아오기(쿼리결과 하나)
					memset(recvBuff, 0, sizeof(recvBuff));
					if(recv(connect_fd, recvBuff, atoi(recv_len),0)<0)
					{
						printf("Client : %s\n",strerror(errno));
					}
					// 쿼리 결과 출력(1 줄씩)
					printf("%s",recvBuff);
				}
				else
				{
					// 쿼리 결과 실패, -1
					printf("Server : %s\n", strerror(errno));
				}
			}
			//show_result(connect_fd, sendBuff, recvBuff, recv_len);
		}
		else
		{	
			//add, del, update의 로그메세지
			if(recv(connect_fd, recv_len, 4, 0)<0)
			{
				printf("DB Server : %s\n", strerror(errno));
			}
			else if(strcmp(recv_len,"ERR")==0)
			{
				printf("DB Server : Query Failed.\n");
			}
			if(strcmp(recv_len,"DML")==0)
			{
				printf("DB Server : Query Success.\n");
			}	
		}
		printf("--------------------------------------------------\n");
	}
}



void 
close_socket(int connect_fd)
{
	close(connect_fd);
	exit(0);
}








