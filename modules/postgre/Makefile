CPPFLAGS=-I /usr/local/pgsql/include
CFLAGS=-W -Wall
LDFLAGS=-L /usr/local/pgsql/lib -lpq

TARGET=p

OBJS=p.o p.h

$(TARGET): $(OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS) -O3

.PHONY: clean
clean:
	@echo "Remove objs...."
	$(RM) $(TARGET) $(OBJS)
	 
