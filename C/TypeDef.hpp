#ifndef __TYPEDEFHPP__
#define __TYPEDEFHPP__

#include <vector>
#include <functional>

namespace NET
{
	typedef struct tagFileEvent {
		int mask;
		void* data;
		unsigned int dataSize;
		::std::function<int(void*, int, int)> readProc;
		::std::function<int(void*, int, int)> writeProc;
	} FILE_EVENT;

	typedef struct tagFiredEvent {
		int fd;
		int mask;
	} FIRED_EVENT;

	typedef struct tagEventLoop {
		int maxfd;
		int size;
		::std::vector<FILE_EVENT> event;
		::std::vector<FIRED_EVENT> fired;
		void* arg;
	} EVENT_LOOP;
}
#endif
