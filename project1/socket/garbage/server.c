#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "wait.h"
//소켓 프로그래밍에 사용될 헤더파일 선언
 
#define BUF_LEN 128
//메시지 송수신에 사용될 버퍼 크기를 선언
 
int main(int argc, char *argv[])
{
    char buffer[BUF_LEN];
    struct sockaddr_in server_addr, client_addr;
    char temp[20];
    int server_fd, client_fd;
    //server_fd, client_fd : 각 소켓 번호
    int len, msg_size;
 
    //에러 : IP주소, 포트번호 미입력
    if(argc != 3)
    {
        printf("usage : %s [IP] [port]\n", argv[0]);
        exit(0);
    }
 
    
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {// 소켓 생성과 동시에, 생성이 불가하면 에러메세지 출력
        printf("Server : Can't open stream socket\n");
        exit(0);
    }
    
    //server_addr 을 NULL로 초기화
    memset(&server_addr, 0x00, sizeof(server_addr));

    //server 셋팅
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); //서버주소, 원래는htonl(INADDR_ANY); 였다. 
    server_addr.sin_port = htons(atoi(argv[2])); //포트번호 입력시, 포트주소
 
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0)
    {//bind() 호출과 동시에, 지정이 불가하면 에러메세지 출력
        printf("Server : Can't bind local address.\n");
        exit(0);
    }
 
    if(listen(server_fd, 5) < 0)
    {//소켓을 수동 대기모드로 설정, 버퍼는 최대 다섯개, 대기상태 실패시 에러메시지 출력
        printf("Server : Can't listening connect.\n");
        exit(0);
    }
 
    //read, write 할 메세지 버퍼 초기화 및 socket, bind, listen 완료
    memset(buffer, 0x00, sizeof(buffer));
    printf("Server : wating connection request.\n");

    while(1) 
    {
	//서버 소켓이 닫힌 상태라면
    	len = sizeof(client_addr);

        if((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len)) <0)
        {
            printf("Server: accept failed.\n");
            exit(0);	//이게 왜 안먹?찌
        }
	//접속한 클라이언트의 ip를 확인하기 위해 inet_ntop 함수  사용
        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
	printf("Sever Socket FD : %d\n",server_fd);
	printf("Server : %s client connected.\n", temp);	//여기서 temp는, client의 IP 주소이다.
   	printf("Client Pid : %d\n",getpid()); 
	//에코가 동작하는 부분이 여기다.
        msg_size = read(client_fd, buffer, 1024);	//클라이언트의 입력값을 받기 위한 함수, read
        write(client_fd, buffer, msg_size);		//클라이언트의 입력값을 서버에 write

        close(client_fd);	//클라이언트의 소켓을 닫습니다.
       	printf("Server : %s client closed.\n", temp);
	int pid=fork();
	if(pid==0){	
		if(getchar()=='\n'){
			printf("자식프로세스 주금");
			exit(0);
		}
	}
	else{
		int status;
		int c = waitpid(pid,&status,WNOHANG);
		if(c==0){
			close(server_fd);
	        	printf("Sever Listening socket closed.\n");
			exit(0);
		}
	}

    }
    //원래는 이자리에  close(server_fd);
//    close(server_fd);	//서버의 소켓을 닫습니다.
//    printf("Sever closed.\n");
    return 0;
}
