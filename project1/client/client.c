#include "client.h"
#include "par.h"

int 
init_socket()
{
	struct sockaddr_in connectSocket;
	int connect_fd;

	memset(&connectSocket, 0, sizeof(connectSocket));

	connectSocket.sin_family = AF_INET;
	inet_aton("192.168.219.100", (struct in_addr*) &connectSocket.sin_addr.s_addr);
	connectSocket.sin_port = htons(12345);

	if((connect_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("%s : Client not make socket.\n",strerror(errno));
		return -1;
	}

	// 연결할 소켓 주소가 잘못 되면, connect 함수에서 block 되어버림.(프로그램 중단)
	// 따라서, 소켓을 생성 후 non-blocking 모드로 바꾸어 주어야 함
	// 이후 select 배열에 등록하고, 타임아웃을 설정한 후 return 값으로 연결을 확인한다.
	if(connect(connect_fd, (struct sockaddr*) &connectSocket, sizeof(connectSocket)) < 0)
	{
        printf("%s : Server not connect.\n", strerror(errno));
		return -1;
	}
	else
	{
		return connect_fd;
	}
}

int
send_query(int connect_fd)
{
	char sendBuff[BUF_SIZE], recvBuff[BUF_SIZE], str[MSG_SIZE], recv_len[4];

	while(1)
	{
		memset(sendBuff, 0, sizeof(sendBuff));
		memset(recvBuff, 0, sizeof(recvBuff));
		char* query;
		char* dml;

		printf("서버에 보낼 Query를 입력하세요: ");
		fgets(str, MSG_SIZE, stdin);
		query = parser(str);

		// [Query Complete] : if client input "bye", process end.  
		if(strncmp(query, "BYE", strlen(query)) == 0)
		{
			return;
		}

		// [Send Query] : send query to server
		strcpy(sendBuff, query);
		if(write(connect_fd, sendBuff, strlen(sendBuff)) < 0)
		{
			printf("Client : %s\n", strerror(errno));
		}

		// [Server Reply]
		if(recv(connect_fd, recvBuff, 51, 0)<0)
		{
			printf("Socket Server : %s\n", strerror(errno));
		}
		printf("%s", recvBuff);
		memset(recvBuff, 0, sizeof(recvBuff));
		memset(recv_len, 0 ,sizeof(recv_len));

		// if DML is "show", server show result to client.
		dml = strtok(str, " ");
		if(strncmp(dml, "show", strlen("show"))==0)
		{	
			// 쿼리 결과받아오기
			// 사실, while 안에 함수를 recv로 해도 되지 않나.
			while(1)
			{
				if((recv(connect_fd, recv_len, 4, 0)) >= 0)
				{	
					// [Check Query Result]
					if(strncmp(recv_len, "NO", strlen("NO")) == 0)
					{
						printf("Query Result >> Empty Result.\n");
						break;
					}
					else if(strncmp(recv_len, "ERR", strlen("ERR")) == 0)
					{	
						printf("DB Server : Query Failed.\n");
						break;
					}
					else if(strncmp(recv_len, "END", strlen("END")) == 0)
					{
						break;
					}

					// [Reply "OK" to server] : ready to receive "show result" data.
					memset(sendBuff, 0, sizeof(sendBuff));
					sprintf(sendBuff, "OK");
					if(send(connect_fd, sendBuff, strlen(sendBuff), 0) < 0)
					{
						printf("Client : %s\n", strerror(errno));
					}

					// 데이터 하나 받아오기(쿼리결과 하나)
					// [Receive to data one by one]
					memset(recvBuff, 0, sizeof(recvBuff));
					if(recv(connect_fd, recvBuff, atoi(recv_len), 0) < 0)
					{
						printf("Client : %s\n", strerror(errno));
					}

					// [Print Query Result] : print one line each
					printf("%s\n", recvBuff);
				}
				else
				{
					// [Fail to Query Result]
					printf("Server : %s\n", strerror(errno));
				}
			}
		}
		else
		{	
			//add, del, update의 로그메세지
			if(recv(connect_fd, recv_len, 4, 0) < 0)
			{
				printf("DB Server : %s\n", strerror(errno));
			}
			else if(strncmp(recv_len, "ERR", strlen("ERR")) == 0)
			{
				printf("DB Server : Query Failed.\n");
			}
			if(strncmp(recv_len, "DML", strlen("DML")) == 0)
			{
				printf("DB Server : Query Success.\n");
			}
		}
		printf("--------------------------------------------------\n");
	}
	return 0;
}

int
close_socket(int connect_fd)
{
	close(connect_fd);

	return 0;
}
