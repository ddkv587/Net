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
    class CSocket;
    class CSocketServer;

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
#include "socket/CSocket.hpp"
#include "socket/CSocketServer.hpp"

// ====== thread ========
#include "CThread.hpp"

// ====== model ========
#include "CBaseModel.hpp"
#include "CModelGroup.hpp"
    
// ====== main ======
#include "CMain.hpp"

#endif
