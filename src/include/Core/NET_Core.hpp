#ifndef __NET_COREHPP__
#define __NET_COREHPP__

namespace NET
{
    // ====== common =======

    // ====== log =======

    // ====== format ========

    // ====== object =====
    class CObject;

    // ====== message =====
    class CBaseMessage;
    class CSystemMessage;

    // ====== task =====
    class CBaseTask;

    // ====== socket =====
    class CSocketBase;
    class CSocketTCP;
    class CSocketTCPClient;
    class CSocketTCPServer;
    class CSocketUDP;
    class CSocketIPC;

    // ====== thread ========
    class CThread;

    // ====== model ========
    class CBaseModel;
    class CModelGroup;

    // ====== main ======
    class CMain;
}

// ====== common =======
#include "TypeDefine.hpp"

// ====== object =====
#include "CObject.hpp"

// ====== message =====
#include "CBaseMessage.hpp"
#include "CSystemMessage.hpp"

// ====== task =====
#include "CBaseTask.hpp"

// ====== socket =====
#include "socket/CSocketBase.hpp"
#include "socket/CSocketTCP.hpp"
#include "socket/CSocketTCPClient.hpp"
#include "socket/CSocketTCPServer.hpp"
#include "socket/CSocketUDP.hpp"
#include "socket/CSocketIPC.hpp"

// ====== thread ========
#include "CThread.hpp"

// ====== model ========
#include "CBaseModel.hpp"
#include "CModelGroup.hpp"

// ====== main ======
#include "CMain.hpp"

#endif
