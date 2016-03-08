TARGET:=client
CC = gcc

all: $(TARGET)

objects = client.o funtion.o interface.o

$(TARGET): $(objects)
	$(CC) -g -o $@ $(objects) -lsqlite3

client.o: client.c client.h config.h funtion.h interface.h
	$(CC) -c client.c

funtion.o: funtion.c funtion.h
	$(CC) -c funtion.c

interface.o: interface.c interface.h
	$(CC) -c interface.c

rebuild: clean all

.PHONY: clean

clean:
	-rm -rf $(TARGET) $(objects)
