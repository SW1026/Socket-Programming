#include "client.h"

int main(int argc, char**argv)
{
	int connect_fd = init_socket();
	send_query(connect_fd);
	close_socket(connect_fd);

	return 0;
}


