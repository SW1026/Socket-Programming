#include "par.h"

char*
parser(char* str)
{
	/*
		add name='ksw',age=27
		del id=3
		update id=3 name='ksw', age=27
		show all
		show id=3 or id=2
	*/
	char* dml;
	char* token;
	static char query[MSG_SIZE];	//전역변수로 설정. 로컬변수로 설정하게되면 주소값만 가져가고 value를 잃어버림
	char* id;
	char* name;
	char* age;
	dml = strtok(str," ");

	if(strncmp(dml, "add", strlen("add")) == 0)
	{
		token = strtok(NULL,"=");
		token = strtok(NULL,",");
		name = token;
		token = strtok(NULL,"=");
		token = strtok(NULL,"\n");
		age = token;
		snprintf(query, sizeof(query), "INSERT INTO employee (name,age) VALUES(%s,%s);", name, age);
	}
	else if(strncmp(dml, "del", strlen("del")) == 0)
	{
		token = strtok(NULL, "\n");
		snprintf(query, sizeof(query), "DELETE FROM employee WHERE %s;", token);
	}
	else if(strncmp(dml, "update", strlen("update")) == 0)
	{
		id = strtok(NULL," ");
		token = strtok(NULL,"\n");
		snprintf(query, sizeof(query), "UPDATE employee SET %s WHERE %s;", token, id);
	}
	else if(strncmp(dml, "show", strlen("show")) == 0)
	{	
		token = strtok(NULL, "\n");
		if(strncmp(token, "all", strlen("all")) == 0)
		{
			snprintf(query, sizeof(query), "SELECT * FROM employee;");
		}
		else
		{
			snprintf(query, sizeof(query), "SELECT * FROM employee WHERE %s;", token);
		}
	}
	else if(strncmp(str, "bye\n", strlen("bye\n")) == 0)
	{
		snprintf(query, sizeof(query), "BYE");
		printf("------------------ Client Reply ------------------\n");
		printf("Query Completed.\n");
		printf("Process exit.\n");
	}
	else
	{
		snprintf(query, sizeof(query), "QUERY FAIL");
		printf("------------------ Client Reply ------------------\n");
		printf("Client : Query input error.\n");	
	}

	return query;
}
