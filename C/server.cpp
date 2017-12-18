#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
	initSocket();	
	while (1)
	{
		client = accept(socket_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addLen);

		if ( client != -1 )
		{
			memset(readBuff, 0, 1024);
			int size = read(socket_fd, readBuff, 1024);
			if(readBuff[0] == 'q') {
				printf("get exit msg\n");
				break;
			} else {
				printf("get msg: %s\n", readBuff);
			}
		}
	}
	destroy();
	return 0;
}
