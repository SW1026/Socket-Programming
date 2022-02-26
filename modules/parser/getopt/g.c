#include "g.h"

//static struct option long_options[];


void parser(struct option *long_options,int argc, char** argv){
	
	int c;
	int option_index;
	while(1){
		c = getopt_long(argc, argv, "",long_options, &option_index);
		
		if(c==-1)
			break;
		char *cols;
		char *token;
		char *name;
		char *age;
		char str[100];//버퍼 조절 필요
		switch (c){
			case 0:	
				break;

			case 'a':
				//name='ksw',age=26 파싱
				token = strtok(optarg,"=");
				token = strtok(NULL,",");
				name=token;
				token = strtok(NULL,"=");
				token = strtok(NULL,"\0");
				age=token;
				sprintf(str,"INSERT INTO employee (name,age) VALUES('%s',%s);\n",name,age);
				break;

			case 'd':
				sprintf(str,"DELETE FROM employee WHERE %s;\n",optarg);
				break;

			case 'u':
				sprintf(str,"UPDATE employee SET %s WHERE %s;\n",argv[3],optarg);
				break;

			case 's':
				if(optarg=="all")
					sprintf(str,"SELECT * FROM employee;\n");
				else{
					sprintf(str,"SELECT %s FROM employee;\n",optarg);
				}
				break;

			case '?':
				//후에 syslog로 바꿔주기
				printf("ERROR OCCURS\n");
				break;
		}

	}
}


int main (int argc, char **argv){
	char ** arv ={
		"./opt",
		"--add",
		"name='ksw',age=27",
		NULL
	};
	parser(long_options, 3, arv);
	return 0;

}
