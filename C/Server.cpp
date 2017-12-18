#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>

#define PORT	8000

static int socket_fd;

void initSocket()
{
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	assert(-1 != socket_fd);

	int on = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	assert( -1 != bind(socket_fd, (struct sockaddr*)&address, sizeof(struct sockaddr_in) ));

	assert( -1 != listen(socket_fd, 10) );
}

void destroy()
{
	if ( socket_fd > 0 ) {
		close(socket_fd);
	}
}

int main(int argc, const char *argv[])
{
	int client;
	struct sockaddr_in client_addr;
	int addLen = sizeof(struct sockaddr_in);
	char readBuff[1024];
	fd_set rfds;
	int ret;
	initSocket();	
	client = accept(socket_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addLen);

	if ( client == -1 ) {
		perror("accept");
	}
	printf("get socket from :%s: %d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
	while (1)
	{
		FD_ZERO(&rfds);
		FD_SET(client, &rfds);

		ret = select(10, &rfds, NULL, NULL, NULL);

		if ( ret == -1 ) {
			perror("select");
		} else {
			if ( FD_ISSET(client, &rfds) ) {
				memset(readBuff, 0, 1024);
				int size = read(client, readBuff, 1024);
				if(readBuff[0] == 'q') {
					printf("get exit msg\n");
					break;
				} else {
					printf("size: %d\n", size);
					printf("get msg[0]: %c\n", readBuff[0]);
					printf("get msg[%d]: %x\n", size-2 , readBuff[size - 2]);
					printf("get msg[%d]: %x\n", size-1 , readBuff[size - 1]);
				}
			}
		}
	}
	destroy();
	return 0;
}
