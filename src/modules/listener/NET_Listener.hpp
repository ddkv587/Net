#ifndef __NETLISTENERHPP__
#define __NETLISTENERHPP__

namespace NET
{
	class CListener;
}

#include <list>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "core/NET_Core.hpp"

#include "Listener.hpp"

#endif
