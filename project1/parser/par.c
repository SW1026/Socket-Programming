#include <stdio.h>
#include <string.h>

char* parser(char * str){
	// add name='ksw',age=27
	// del id=3
	// update id=3 name='ksw', age=27
	// show all
	// show id=3 or id=2
	char * dml;
	char * token;
	static char query[256];
	char * id;
	char * name;
	char * age;
	dml = strtok(str," ");
	
	if(strcmp(dml,"add")==0){
		token = strtok(NULL,"=");
		token = strtok(NULL,",");
		name=token;
		token = strtok(NULL,"=");
		token = strtok(NULL,"\n");
		age=token;
		sprintf(query,"INSERT INTO employee (name,age) VALUES(%s,%s);",name,age);
	}
	else if(strcmp(dml,"del")==0){
		token = strtok(NULL,"\n");
		sprintf(query,"DELETE FROM employee WHERE %s;",token);
	}
	else if(strcmp(dml,"update")==0){
		id = strtok(NULL," ");
		token = strtok(NULL,"\n");
		sprintf(query,"UPDATE employee SET %s WHERE %s;",id,token);
	}
	else if(strcmp(dml,"show")==0){	
		token = strtok(NULL, "\n");	
		if(strcmp(token,"all")==0)
			sprintf(query,"SELECT * FROM employee;");
		else
			sprintf(query,"SELECT * FROM employee WHERE %s;",token);
	}
	else{
		printf("query input error.\n");
	}
	printf("Query : %s\n",query);
	//이 부분이 세그맨테이션 오류
	return query;
}


 
int main(){
	char str[256];
	fgets(str,256,stdin);

	char * ptr = parser(str);
	printf("%s\n",ptr);
	return 0;
}

