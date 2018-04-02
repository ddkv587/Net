#ifndef __NETSOCKETHPP__
#define __NETSOCKETHPP__

namespace NET_Socket
{
	class CSocket;

	class CSocketServer;
}

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <string.h>

#include "log/NET_Log.hpp"

#include "Socket.hpp"
#include "SocketServer.hpp"

#endif
