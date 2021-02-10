client: main.o c.o par.o
	@gcc -o client main.o c.o par.o

main.o : c.h par.h main.c
c.o : c.h par.h c.c
par.o : par.h par.c

.PHONY:
clean :	
	@rm -f main.o c.o par.o

