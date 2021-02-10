#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {

	int c;
	extern char *optarg;
	extern int optind;

	argc=3;
	char**arv = argv;
	while( (c=getopt(argc, arv, ":abc:d:")) != -1) {

		//printf("Current Option Number: %d\n",optind);
		
		switch(c) {
			case 'a':
				printf("Option : a");
				break;

			case 'b':
				printf("Option : b");
				break;

			case 'c':
				printf("Option : c, Argument : %s\n",optarg);
				break;

			case 'd':
				printf("Option : d, Argument : %s\n",optarg);

			case ':':	
				if(optopt=='c')
					printf("c 옵션의 인자를 넣지 않음");

				if(optopt=='d')
					printf("d 옵션의 인자를 넣지 않음");
				break;

			case '?':
				printf("Option Error!! No Option : %c",optopt);
				break;
		}
	//	printf("\tNext Option Number : %d\n",optind+1);
	}
	
	return 0;
}



