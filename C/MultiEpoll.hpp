#ifndef __MULTIEPOLLHPP__
#define __MULTIEPOLLHPP__

#include "Multiplex.hpp"

class CMultiEpoll : CMultiplexManager::IMultiplex
{
	public:
		int 	addFileEvent(int, int);
		void	delFileEvent(int, int);
		int 	eventLoop(EVENT_LOOP*, int);
};
#endif
