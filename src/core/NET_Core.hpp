#ifndef __NET_COREHPP__
#define __NET_COREHPP__

namespace NET
{
	//====== log =======
	class CLog;

    //====== format ========
    class XMLHelper;
    class XMLElement;

	//====== load helper =====
	class CLoadHelper;
    
    //====== thread ========
    class CThreadBase;

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
#include <fstream>
#ifndef DEF_ASSERT
#include <assert.h>
#endif

#include "platform/TypeDefine.hpp"

//============ log ==============
#include <glog/logging.h>
using namespace google;

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "log/Log.hpp"
extern int errno;

//=========== format =========
#include "rapidxml/rapidxml.hpp"
#include "XMLElement.hpp"
#include "XMLHelper.hpp"

//=========== load helper =========
#include <dlfcn.h>
#include "loadHelper/CLoadHelper.hpp"

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

//========== platform ==============
#include "multiplex/MultiStruct.hpp"
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
