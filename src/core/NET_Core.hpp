#ifndef __NET_COREHPP__
#define __NET_COREHPP__

namespace NET
{
	//====== common =======
	class CMD;

	//====== log =======
	class CLog;

    //====== format ========
    class XMLElement;
    class XMLHelper;

	//====== config =====
	class ConfigParser;
    
	//====== load helper =====
	class CLoadHelper;

    //====== thread ========
    class CThreadBase;

	//====== socket ===========
	class CSocket;
	class CSocketServer;
        
    //====== timer ============
    class CTimer;
    class IClientListener;

	//======= multiplex ============
	class IMultiBase;
	class CMultiManager;
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
#include <mutex>
#ifndef DEF_ASSERT
#include <assert.h>
#endif

#include "platform/TypeDefine.hpp"
#include "platform/Command.hpp"

//============ log ==============
#include <glog/logging.h>
using namespace google;

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "log/Log.hpp"
extern int errno;

//=========== format =========
#include "format/rapidxml/rapidxml.hpp"
#include "format/rapidxml/rapidxml_utils.hpp"
#include "format/rapidxml/rapidxml_print.hpp"
using namespace rapidxml;
#include "format/XMLElement.hpp"
#include "format/XMLHelper.hpp"

//====== config =====
#include "config/ConfigParser.hpp"

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
#include <unistd.h>
#include <fcntl.h>

#include "socket/Socket.hpp"
#include "socket/SocketServer.hpp"
#include "socket/IClientListener.hpp"

#include "timer/ITimerListener.hpp"
#include "timer/Timer.hpp"

//========== platform ==============
#include "platform/multiplex/MultiStruct.hpp"
#include "platform/multiplex/MultiBase.hpp"

#ifdef OS_BSD
#include <sys/types.h>
#include <sys/event.h>
#include "platform/multiplex/MultiKqueue.hpp"
#else
#include <sys/epoll.h>
#include "platform/multiplex/MultiEpoll.hpp"
#include "platform/multiplex/MultiSelect.hpp"

#endif

#endif
