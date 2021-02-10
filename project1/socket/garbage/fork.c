#include <stdio.h>
#include <stdlib.h>

int main(void){
	pid_t pid;
	pid = fork();
	if(pid==-1){
		printf("This is Error msg.\n");
		exit(0);
	}
	
	if(pid!=0){
		printf("Parent: %d %d %d\n",getpid(),getppid(),pid);
		exit(0);
	}
	else{
		printf("Child: %d %d %d\n",getpid(),getppid(),pid);
		exit(0);
	}

	return 0;
}
