CPPFLAGS=-I /usr/local/pgsql/include
CFLAGS=-W -Wall
LDFLAGS=-L /usr/local/pgsql/lib -lpq


TARGET=server
OBJS=main.o s.o p.o

$(TARGET) : $(OBJS)
	@gcc $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

main.o : s.h p.h main.c
s.o : s.h p.h s.c
p.o : p.h p.c

.PHONY:
clean : 
	@rm -rf $(TARGET) $(OBJS)
