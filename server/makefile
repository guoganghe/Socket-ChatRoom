TARGET:=server

all: $(TARGET)

$(TARGET):server.c
	gcc server.c -o $@

rebuild: clean all

.PHONY: clean
clean:
	-rm -rf $(TARGET)

