TARGET:=server

all: $(TARGET)

$(TARGET):server.c
	gcc server.c server.h -lpthread -o $@

rebuild: clean all

.PHONY: clean
clean:
	-rm -rf $(TARGET)

