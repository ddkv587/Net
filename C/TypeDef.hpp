#ifndef __TYPEDEFHPP__
#define __TYPEDEFHPP__

#include <vector>
#include <functional>

namespace NET
{
	typedef struct tagFileEvent {
		int fd;
		int mask;
		void* data;
		unsigned int dataSize;
		::std::function<int(struct tagFileEvent*)> readProc;
		::std::function<int(struct tagFileEvent*)> writeProc;
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

	extern "C" {
		int readProc(struct tagFileEvent* file)
		{
			if ( NULL == file ) return -1;

			return -1;
		}

		int writeProc(struct tagFileEvent* file)
	}
}
#endif
