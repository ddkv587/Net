#ifndef __MULTIEPOLLHPP__
#define __MULTIEPOLLHPP__

#include "Multiplex.hpp"

namespace NET
{
	class CMultiEpoll : CMultiBase
	{
		public:
			CMultiEpoll();
			virtual ~CMultiEpoll();

			int 	addFileEvent(int, int);
			void	delFileEvent(int, int);
			int 	eventLoop(void*);

			private:
			int m_epfd;
			struct epoll_event *m_pEvents;
	};
}
#endif
