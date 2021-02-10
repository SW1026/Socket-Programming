//서버 프로그램
#include "s.h"

void init_socket(){

	char readBuff[BUF_SIZE],sendBuff[BUF_SIZE];
	struct sockaddr_in server_addr, client_addr;
	char client_ip[20];
	int listen_fd, connect_fd;
	int client_len;

	if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		printf("Server : can't open stream socket\n");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(12345);

	if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))<0){
		printf("Server : can't bind local address.\n");
		exit(1);
	}

	if(listen(listen_fd, 5)<0){
		printf("Server : can't listening connect.\n");
		exit(1);
	}

	memset(readBuff, 0, sizeof(readBuff));
	printf("Server : waiting connection: request.\n");
	while(1){
		client_len = sizeof(client_addr);
		if(((connect_fd = accept(listen_fd,(struct sockaddr*)&client_addr, &client_len)))<0){
			printf("Server : accept failed.\n");
			exit(1);
		}
		
		int pid = fork();
		if(pid == 0){//자식서버
			close(listen_fd); //자식서버의 리슨소켓을 닫아준다.
			inet_ntop(AF_INET,&client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
			printf("Server : %s clinet connected.\n",client_ip);
			//msg_size = read(connect_fd, readBuff, 1024);
			//write(connect_fd, readBuff, msg_size);
			
			ssize_t receivedBytes;
	 
			// read할 값이 있다면 계속 읽어들인다.
			while((receivedBytes = read(connect_fd, readBuff, MSG_SIZE)) > 0)
			{                
	 
			    printf("%lu bytes read\n", receivedBytes);
			    readBuff[receivedBytes] = '\0';
			    fputs(readBuff, stdout);
			    fflush(stdout);
	    
			    sprintf(sendBuff,"%s",readBuff);
			    if(strcmp(sendBuff,"aa\n")==0)
				    sprintf(sendBuff,"");
			    write(connect_fd, sendBuff, strlen(sendBuff));
			}
			close(connect_fd);//통신이 끝나서 자식서버의 커넥트소켓을 닫아준다.
			printf("Server : %s client closed.\n",client_ip);
			exit(0);	//자식프로세스 정상종료
		}
		else{//부모서버.  커넥트 소켓을 닫아준다.
			close(connect_fd);
		}
	}
	close(listen_fd);
	exit(0);	
}


int main(){

	init_socket();
	return 0;

}



