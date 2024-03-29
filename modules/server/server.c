#include "server.h"
#include "postgres.h"

void 
init_socket()
{
	if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Socket Server : can't open stream socket\n");
		exit(1);
	}
}

void 
listen_socket()
{
	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(12345);

	if(bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		printf("Socket Server : can't bind local address.\n");
		exit(1);
	}

	if (listen(listen_fd, 5) < 0)
	{
		printf("Socket Server : can't listening connect.\n");
		exit(1);
	}
}

void 
connect_socket()
{
	while(1)
	{
		// 서버자체는 계속 열려있음, 즉 데이터를 받아들일 준비를 하고 있어야 함.
		client_len = sizeof(client_addr);
		if((connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len)) < 0)
		{
			printf("Socket Server : accept failed.\n");
			exit(1);
		}
		int pid = fork();
		if(pid == 0)
		{
			// 자식서버
			// 자식서버의 리슨소켓을 닫아준다.
			close(listen_fd);
			inet_ntop(AF_INET,&client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
			printf("Socket Server : %s clinet connected.\n",client_ip);

			char recvBuff[MSG_SIZE], sendBuff[BUF_SIZE];
			PGconn* conn = NULL;
			ConnStatusType conn_status; 

			// #1 디비 연결
			conn = connect_testdb();
			conn_status = PQstatus(conn);
			
			while(1)
			{	
				// 쿼리를 반복해서 읽는다. (클라이언트가 쿼리를 보낼때마다 읽어들인다.)
				int recvBytes;
				if((recvBytes = recv(connect_fd, recvBuff, MSG_SIZE,0)) > 0)
				{
					if(recvBytes>=10 && strcmp(recvBuff,"QUERY FAIL")==0)
					{
						syslog(LOG_ERR,"Client Query WRONG.");			
					}
					if(recvBytes>=3 && strcmp(recvBuff,"BYE")==0)
					{
						syslog(LOG_INFO,"Client Bye.");
						break;
					}
					memset(sendBuff,0,sizeof(sendBuff));
					struct Result* result = NULL;

					// #2 디비에 쿼리를 날려준다. (에러가 나도 함수는 리턴한다.)
					result = query_employee(conn, recvBuff);

					// #3 클라이언트에 결과를 돌려준다.
					send_result(connect_fd, result, sendBuff, recvBuff);	
					printf("--------------------------------------------------\n");
					//전송이 끝나고 recvBuff 초기화
					memset(recvBuff,0,sizeof(recvBuff));
				}
				else
				{	
					syslog(LOG_ERR, strerror(errno));
					break;
				}
			}

			// 클라이언트에서 종료해서 종료함.
			// 자식서버 통신 마무리
			close_socket(conn);
		}
		else
		{
			//부모서버.  커넥트 소켓을 닫아준다.
			close(connect_fd);
			while((waitpid(-1,NULL,WNOHANG)) > 0)
			{
				syslog(LOG_INFO,"자식 종료함");
			}
		}
	}
	close(listen_fd);
	exit(0);
}

void 
send_result(int connect_fd, struct Result* result, char* sendBuff, char* recvBuff)
{
	char send_len[4];
	memset(recvBuff, 0, sizeof(recvBuff));
	memset(send_len, 0, sizeof(send_len));

	snprintf(sendBuff, sizeof(sendBuff), "------------------ Server Reply ------------------\n");
	send(connect_fd, sendBuff, 51,0);

	if(strncmp(result->type, "FAIL QUERY", strlen("FAIL QUERY")) == 0)
	{	
		// 쿼리문이 아님
		memset(send_len, 0, sizeof(send_len));
		snprintf(send_len, strlen("ERR"), "ERR");
		send(connect_fd, send_len, 4, 0);		
	}
	else if(strncmp(result->type, "NOT SELECT", strlen("NOT SELECT")) == 0)
	{	
		//add, del, update
		memset(send_len, 0, sizeof(send_len));
		snprintf(send_len, strlen("DML"), "DML");
		send(connect_fd, send_len, 4, 0);
	}
	else
	{	
		//show
		if(strncmp(result->type, "EMPTY RESULT", strlen("EMPTY RESULT")) == 0)
		{	
			// 결과가 비었을 경우
			printf("Query Result>> Empty Result.\n");
			memset(send_len, 0, sizeof(send_len));
			snprintf(send_len, strlen("NO"), "NO");
			send(connect_fd, send_len, 4, 0);
		}
		else if(strncmp(result->type, "SELECT RESULT", strlen("SELECT RESULT")) == 0)
		{	
			// 결과가 있을 경우
			int i;
			for(i=0; i < result->rows; ++i)
			{
				// [Check Query Result Length] : 결과(1 줄) 데이터 길이 송신
				snprintf(send_len, sizeof(send_len), "%d", strlen(result->res[i]));
				send(connect_fd, send_len, strlen(send_len), 0);

				// 데이터 보내도 되는지 응답 확인
				memset(recvBuff, 0, strlen(recvBuff));
				recv(connect_fd, recvBuff, 2, 0);

				if(strncmp(recvBuff, "OK", strlen("OK")) == 0)
				{
					memset(sendBuff, 0, sizeof(char) * BUF_SIZE);
					snprintf(sendBuff, strlen(result->res[i]), "%s", result->res[i]);
					// 쿼리 결과 1줄씩, 데이터 보냄
					send(connect_fd, sendBuff, atoi(send_len), 0);
				}
			}
			memset(send_len, 0, sizeof(send_len));
			snprintf(send_len, sizeof(send_len), "END");
			send(connect_fd, send_len, 4, 0);
		}
	}
}

void
close_socket(PGconn* conn)
{
	PQfinish(conn);	//통신이 끝나서 DB를 닫아준다.

	printf("DB Server : DB closed.\n");

	close(connect_fd);//통신이 끝나서 자식서버의 커넥트소켓을 닫아준다.

	printf("Socket Server : %s client closed.\n", client_ip);
	printf("--------------------------------------------------\n");

	exit(0);	//자식프로세스 정상종료
}
