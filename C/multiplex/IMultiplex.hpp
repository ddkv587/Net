#ifndef __IMULTIPLEXHPP__
#define __IMULTIPLEXHPP__

namespace NET
{
	class IMultiplex
	{
		public:
			virtual int 	resize(int size);
			virtual int 	addFileEvent(int, int) = 0;
			virtual void	delFileEvent(int, int) = 0;
			virtual int 	eventLoop(EVENT_LOOP*, void *) = 0;
	};
}
#endif
