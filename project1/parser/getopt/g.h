#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>


void parser(struct option *long_options,int argc, char** argv);

int age;
static struct option long_options[] = 
{
	{"add",1,0,'a'},
	{"del",1,0,'d'},
	{"update",1,0,'u'},
	{"show",1,0,'s'},
	

	{0, 0, 0, 0}
};

//3번째 플래그 필드가 0또는 NULL이면 뒤에 있는 숫자를 리턴한다.

