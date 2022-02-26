CPPFLAGS=-I /usr/lib64/pgsql/include
CFLAGS=-W -Wall
LDFLAGS=-L /var/lib/pgsql -lpq

TARGET=server
OBJS=main.o server.o postgres.o

$(TARGET) : $(OBJS)
	@gcc $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

main.o : server.h postgres.h main.c
s.o : server.h postgres.h server.c
p.o : postgres.h postgres.c

.PHONY:
clean : 
	@rm -rf $(TARGET) $(OBJS)
