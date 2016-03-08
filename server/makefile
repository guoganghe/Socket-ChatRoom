TARGET:=server
CC = gcc

all: $(TARGET)

objects = server.o funtion.o

$(TARGET): $(objects)
	$(CC) -g -o $@ $(objects) -lsqlite3 -lpthread

server.o: server.c server.h config.h funtion.h
	$(CC) -c server.c 

funtion.o: funtion.c funtion.h
	$(CC) -c funtion.c

rebuild: clean all

.PHONY: clean

clean:
	-rm -rf $(TARGET) $(objects)
