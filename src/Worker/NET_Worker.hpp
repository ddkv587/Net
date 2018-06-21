#ifndef __NETPROCESSORHPP__
#define __NETPROCESSORHPP__

namespace NET
{
	class CWorker;
}

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>

#include "core/NET_Core.hpp"
#include "listener/NET_Listener.hpp"
#include "protocol/NET_Protocol.hpp"

#include "Processor.hpp"

#endif
