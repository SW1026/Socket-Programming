#include "par.h"

char * parser(char * str)
{
	// add name='ksw',age=27
	// del id=3
	// update id=3 name='ksw', age=27
	// show all
	// show id=3 or id=2
	char * dml;
	char * token;
	static char query[MSG_SIZE];	//전역변수로 설정. 로컬변수로 설정하게되면 주소값만 가져가고 value를 잃어버림
	char * id;
	char * name;
	char * age;
	dml = strtok(str," ");

	// strncmp 로 변경 해야 함
	if(strcmp(dml,"add")==0)
	{
		token = strtok(NULL,"=");
		token = strtok(NULL,",");
		name=token;
		token = strtok(NULL,"=");
		token = strtok(NULL,"\n");
		age=token;
		sprintf(query,"INSERT INTO employee (name,age) VALUES(%s,%s);",name,age);
	}
	else if(strcmp(dml,"del")==0)
	{
		token = strtok(NULL,"\n");
		sprintf(query,"DELETE FROM employee WHERE %s;",token);
	}
	else if(strcmp(dml,"update")==0)
	{
		id = strtok(NULL," ");
		token = strtok(NULL,"\n");
		sprintf(query,"UPDATE employee SET %s WHERE %s;",token,id);
	}
	else if(strcmp(dml,"show")==0)
	{	
		token = strtok(NULL, "\n");
		if(strcmp(token,"all")==0)
		{
			sprintf(query,"SELECT * FROM employee;");
		}
		else
		{
			sprintf(query,"SELECT * FROM employee WHERE %s;",token);
		}
	}
	else if(strcmp(str,"bye\n")==0)
	{
		sprintf(query,"BYE");
		printf("------------------ Client Reply ------------------\n");
		printf("Query Completed.\n");
		printf("Process exit.\n");
	}
	else
	{
		sprintf(query,"QUERY FAIL");
		printf("------------------ Client Reply ------------------\n");
		printf("Client : Query input error.\n");	
	}

	//printf("Query : %s\n",query);
	return query;
}


 

