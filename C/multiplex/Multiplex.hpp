#ifndef __MULTIPLEXMANAGERHPP__
#define __MULTIPLEXMANAGERHPP__

#include "TypeDef.hpp"
#include "common.h"
#include "IMultiplex.hpp"
#ifdef OS_BSD
#include "MultiKqueue.hpp"
#else
#include "MultiEpoll.hpp"
#include "MultiSelect.hpp"
#endif

DEFINE_ONCE(NET_READABLE, 0x01)
DEFINE_ONCE(NET_WRITABLE, 0x02)

namespace NET
{
	class CMultiplexManager
	{
		public:
			enum EMultip_Type
			{
				EMT_NONE = -1,
				EMT_EPOLL,
				EMT_KQUEUE,
				EMT_SELECT,
			};

		public:
			CMultiplexManager(EMultip_Type type = EMT_EPOLL, int size = SYSTEM_MAX_EVENTS);
			virtual ~CMultiplexManager();

			void 	resizeEvents(int size);
			void	switchType(EMultip_Type);

			void 	addFileEvent(int fd, int mask);
			void 	delFileEvent(int fd, int mask);
			int		eventLoop(void* timeout);

			EVENT_LOOP* getEventLoop() { return m_pEventLoop; } 
			
			CMultiplexManager(CMultiplexManager&) = delete;
			CMultiplexManager(const CMultiplexManager&) = delete;

		private:
			EVENT_LOOP* m_pEventLoop;
			IMultiplex* m_pHandler;
	};
}
#endif
