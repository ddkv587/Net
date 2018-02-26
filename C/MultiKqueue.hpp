#ifndef __MULTIKQUEUEHPP__
#define __MULTIKQUEUEHPP__

#include <sys/types.h>
#include <sys/event.h>
#include "IMultiplex.hpp"

class CMultiKqueue : IMultiplex
{
	public:
		CMultiKqueue(int size);
		virtual ~CMultiKqueue();

		int 	addFileEvent(int, int);
		void	delFileEvent(int, int);
		int 	eventLoop(EVENT_LOOP*, void*);
	
	private:
		int m_kqfd;
		struct kevent *m_events;
}

#endif
