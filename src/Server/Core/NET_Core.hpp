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
    class CThread;

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

//============ base start ==============
#include <vector>
#include <fstream>
#include <mutex>
#ifndef DEF_ASSERT
#include <assert.h>
#endif

#include "Common/TypeDefine.hpp"
#include "Common/Command.hpp"

// log
#include <glog/logging.h>
using namespace google;

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "log/Log.hpp"
extern int errno;

// format
#include "Common/format/rapidxml/rapidxml.hpp"

// socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "socket/Socket.hpp"

// thread
#include <thread>
#include "thread/ThreadBase.hpp"
//============ base end ==============

//============ config ==============
#include "config/ConfigParser.hpp"

#include "socket/SocketServer.hpp"
#include "socket/IClientListener.hpp"

#include "timer/ITimerListener.hpp"
#include "timer/Timer.hpp"

//============ event ==============
#include "event/CBaseEvent.hpp"
#include "event/CFileEvent.hpp"
#include "event/CTimeEvent.hpp"

//============ module ==============
#include "module/CBaseModule.hpp"
#include "module/CModuleManager.hpp"

//============ multiplex ==============
#include "multiplex/MultiStruct.hpp"
#include "multiplex/MultiManager.hpp"

#ifdef OS_BSD
#include <sys/types.h>
#include <sys/event.h>
#include "multiplex/MultiKqueue.hpp"
#else
#include <sys/epoll.h>
#include "multiplex/MultiEpoll.hpp"
#include "multiplex/MultiSelect.hpp"
#endif

//============ protocol ==============
#include "protocol/IProtocol.hpp"
#include "protocol/ProtocolBase.hpp"

//============ server ==============
#include "server/IClientListener.hpp"
#include "server/SocketServer.hpp"

//============ timer ==============
#include "timer/ITimerListener.hpp"
#include "timer/Timer.hpp"

#endif
