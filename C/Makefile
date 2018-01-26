CC	:= g++

SRC_SERVER		:= main.cpp Listener.cpp Processer.cpp
LIBS_SERVER		:= -lpthread
CFLAGS_SERVER	:= -std=c++11 -g

SRC_CLIENT		:= client.cpp
LIBS_CLIENT		:= -lpthread -lc
CFLAGS_CLIENT	:= -std=c++11 -g -DPING

all: server client

server:
	$(CC) $(LIBS_SERVER) $(CFLAGS_SERVER) $(SRC_SERVER) -o server

client:
	$(CC) $(LIBS_CLIENT) $(CFLAGS_CLIENT) $(SRC_CLIENT) -o client

clean:
	rm -rf server
	rm -rf client

.PHONY:clean
