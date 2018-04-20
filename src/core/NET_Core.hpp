#ifndef __NET_COREHPP__
#define __NET_COREHPP__

namespace NET
{
	//====== log =======
	class CLog;

	//====== thread ========
	class CThreadBase;

	//====== load helper =====
	class CLoadHelper;

	//====== socket ===========
	class CSocket;
	class CSocketServer;

	//======= multiplex ============
	class CMultiBase;
#ifdef OS_BSD
	class CMultiKqueue;
#else
	class CMultiEpoll;
	class CMultiSelect;
#endif
}

//============ common ==============
#include <vector>
#ifndef DEF_ASSERT
#include <assert.h>
#endif

#define CHECK_R(X, R) \
	do { if (!(X)) return (R); } while(0)

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define handle_error_errno(en, msg) \
	do { errno = en, perror(msg); exit(EXIT_FAILURE); } while(0)

#define NET_NONE 		0x00
#define NET_READABLE 	0x01
#define NET_WRITABLE 	0x02

#define SYSTEM_VERSION					0.1 
#define SYSTEM_MAX_EVENTS 				1024
#define SYSTEM_SOCKET_PORT				8000

#define POLL_TIMEOUT_SEC 				5
#define POLL_TIMEOUT_USEC 				0

//============ xml ==============
#include "xml/rapidxml.hpp"

//============ log ==============
#include <glog/logging.h>
using namespace google;

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "log/Log.hpp"

extern int errno;

//=========== load helper =========
#include <dlfcn.h>
#include "loadHelper/LoadHelper.hpp"

//=========== thread ===============
#include <thread>
#include "thread/ThreadBase.hpp"

//=========== socket ================
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <string.h>

#include "socket/Socket.hpp"
#include "socket/SocketServer.hpp"

//========== multiplex ==============
#include "multiplex/TypeDef.hpp"
#include "multiplex/MultiBase.hpp"

#ifdef OS_BSD
#include <sys/types.h>
#include <sys/event.h>
#include "multiplex/MultiKqueue.hpp"
#else
#include <sys/epoll.h>
#include "multiplex/MultiEpoll.hpp"
#include "multiplex/MultiSelect.hpp"
#endif

#endif
