client: main.o client.o par.o
	@gcc -o client main.o client.o par.o

main.o : client.h par.h main.c
client.o : client.h par.h client.c
par.o : par.h par.c

.PHONY:
clean :	
	@rm -f main.o client.o par.o client

