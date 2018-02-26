#ifndef __MULTIEPOLLHPP__
#define __MULTIEPOLLHPP__

#include "IMultiplex.hpp"
class CMultiEpoll : CMultiplexManager::IMultiplex
{
	public:
		CMultiEpoll();
		virtual ~CMultiEpoll();

		int 	addFileEvent(int, int);
		void	delFileEvent(int, int);
		int 	eventLoop(EVENT_LOOP*, int);
};
#endif
