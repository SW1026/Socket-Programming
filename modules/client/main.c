#include "client.h"

int main(int argc, char**argv)
{
	int connect_fd;
	if((connect_fd = init_socket()) < 0)
	{
		exit(1);
	}
	if(send_query(connect_fd) < 0)
	{
		exit(1);
	}
	if(close_socket(connect_fd) < 0)
	{
		exit(1);
	}

	return 0;
}


