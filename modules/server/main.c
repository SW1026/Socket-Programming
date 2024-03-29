#include <libgen.h>
#include "server.h"
#include "postgres.h"

int 
main(int argc, char* argv[])
{	
	//데몬 프로세스로 만들어준다.
	daemon(0,0);

	// syslog를 LOG_ERR 단계(0~3) 까지 로그 범위 설정. 
	setlogmask(LOG_UPTO(LOG_ERR)); 
	openlog(basename(argv[0]), LOG_PERROR | LOG_CONS, LOG_USER);

	//소켓 연결
	init_socket();
	listen_socket();

	// DB 연동
	connect_socket();

	closelog();
	return 0;
}
