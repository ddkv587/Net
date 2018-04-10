#ifndef __NETPROCESSORHPP__
#define __NETPROCESSORHPP__

namespace NET
{
	class CProcessor;
}

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"
#include "ThreadBase.hpp"
#include "listener/NET_Listener.hpp"
#include "multiplex/NET_Multiplex.hpp"
#include "protocol/NET_Protocol.hpp"

#include "Processor.hpp"

#endif
