#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "../common.h"
#include "Protocol.hpp"

#define CONNECT_CLIENT 1

int main(int argc, const char *argv[])
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	assert(-1 != fd);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(SYSTEM_SOCKET_PORT);
	
	assert(-1 != connect(fd, (struct sockaddr*)&address, sizeof(struct sockaddr_in)));
	
	char buff[1024];
	memset(buff, 0, 1024);

	HEADER_MANAGER *header = (HEADER_MANAGER*)buff;

#ifdef ECHO
	header->sync = SYNC_FLAG;
	header->protocol = EP_ECHO;
	header->size = 6;

	char* body = buff + sizeof(HEADER_MANAGER);
	strncpy(body, "hello!", 6);

	int size = send(fd, buff, 6 + sizeof(HEADER_MANAGER), 0);
	std::cout<<"send bytes: "<<size<<std::endl;

	memset(buff, 0, 1024);
	size = recv(fd, buff, 1024, 0);
	std::cout<<"recv bytes: "<< size << " : " << buff <<std::endl;
#endif

#ifdef PING
	header->sync = SYNC_FLAG;
	header->protocol = EP_PING;
	
	PING_MANAGER* ping_header = (PING_MANAGER*)(buff + sizeof(HEADER_MANAGER));
	ping_header->current = 0;
	ping_header->deadLimit = 10000;
	char* body = (char*)(buff + sizeof(HEADER_MANAGER) + sizeof(PING_MANAGER));
	strncpy(body, "hello!", 6);

	header->size = sizeof(PING_MANAGER) + 6;

	int size = send(fd, buff, header->size + sizeof(HEADER_MANAGER), 0);
	std::cout<<"send bytes: "<<size<<std::endl;

	while (1)
	{
		memset(buff, 0, 1024);
		size = recv(fd, buff, 1024, 0);
		std::cout<<"recv bytes: "<< size << " : " << buff <<std::endl;
		if ( size == 0 ) break; 

		HEADER_MANAGER* header = (HEADER_MANAGER*)buff;
		if ( header->sync == SYNC_FLAG )
		{
			if ( header->protocol == EP_PING )
			{
				PING_MANAGER* ping_header = (PING_MANAGER*)(buff+sizeof(HEADER_MANAGER));
					
				printf("recv ping current: %d", ping_header->current);

				ping_header->current += 1;
				send(fd, buff, size, 0);

				if ( ping_header->current >= ping_header->deadLimit )
				{
					printf("current end: %d", ping_header->current);
					break;
				}
			}
		}
	}
#endif
	return 0;
}
