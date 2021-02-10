//서버 프로그램
#include "s.h"	//sokcet
#include "p.h"	//postgreSQL
// p.h : ...  connect_testdb(),query_employee,PQfinish() 사용



void init_socket(){
	if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		printf("Socket Server : can't open stream socket\n");
		exit(1);
	}
}

void listen_socket(){
	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(12345);

	if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))<0){
		printf("Socket Server : can't bind local address.\n");
		exit(1);
	}

	if(listen(listen_fd, 5)<0){
		printf("Socket Server : can't listening connect.\n");
		exit(1);
	}
	memset(readBuff, 0, sizeof(readBuff));
}


void connect_socket(){
	while(1){
		client_len = sizeof(client_addr);
		if(((connect_fd = accept(listen_fd,(struct sockaddr*)&client_addr, &client_len)))<0){
			printf("Socket Server : accept failed.\n");
			exit(1);
		}
		int pid = fork();
		if(pid == 0){//자식서버
			close(listen_fd); //자식서버의 리슨소켓을 닫아준다.
			inet_ntop(AF_INET,&client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
			printf("Socket Server : %s clinet connected.\n",client_ip);

			// #1 디비 연결
			PGconn* conn = connect_testdb();
			ssize_t receivedBytes;
	 
			//read 할 값이 있다면 계속 읽어들인다.
			while((receivedBytes = read(connect_fd, readBuff, MSG_SIZE)) > 0){
			        readBuff[receivedBytes] = '\0';
				
				//읽어들인 값을 서버 화면에 출력해준다.
				printf("client request : "); 
				fputs(readBuff, stdout);
				printf("\n");
				
				// #2 디비에 쿼리를 날려준다.
				char (*res)[128] = query_employee(conn, readBuff);
				// #3 클라이언트에 결과를 돌려준다.
				send_result(connect_fd, sendBuff, res);	

				fflush(stdout); 	//개행문자 등 출력버퍼를 비워준다.
				printf("-----------------------------------------------\n");

			}
			close_socket(conn);
		}
		else{//부모서버.  커넥트 소켓을 닫아준다.
			close(connect_fd);
		}
	}
	close(listen_fd);
	exit(0);	
}

void send_result(int connect_fd, char* sendBuff, char** r){
	//클라이언트에 써준다.
	char (*res)[128] = r;
	sprintf(sendBuff,"------------------ Server Reply ------------------\n");
	write(connect_fd, sendBuff, strlen(sendBuff));
			
	printf("커넥트소켓: %d",connect_fd);
	if(strcmp(res[0],"NOT QUERY")==0){	//쿼리문이 아님
		sprintf(sendBuff,"Query Failed.\n");
		write(connect_fd, sendBuff, strlen(sendBuff));
	}
	else if(strcmp(res[0],"NOT SELECT")==0){	//add,del,update
		sprintf(sendBuff,"Query Success.\n");
		write(connect_fd, sendBuff, strlen(sendBuff));

	}
	else{	//show(SELECT)
		if(strcmp(res[0],"EMPTY RESULT")==0){	//결과가 비었을 경우
			sprintf(sendBuff,"Query Result>> Empty Result.\n");
			write(connect_fd, sendBuff, strlen(sendBuff));
		}
		else{	//결과가 있을 경우
			for(int i=1; i<=res[0][0]; ++i){	//res[0][0]에 값이 있는 배열의 길이를 넣어두었다.
				sprintf(sendBuff,"%s",res[i]);
				write(connect_fd, sendBuff, strlen(sendBuff));
			}
		}
	}
	sprintf(sendBuff,"\n");
	write(connect_fd, sendBuff, strlen(sendBuff));
}

void close_socket(PGconn * conn){
	PQfinish(conn);	//통신이 끝나서 DB를 닫아준다.
	printf("DB Server : DB closed.\n");
	close(connect_fd);//통신이 끝나서 자식서버의 커넥트소켓을 닫아준다.
	printf("Socket Server : %s client closed.\n",client_ip);
	exit(0);	//자식프로세스 정상종료
}
