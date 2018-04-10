#ifndef __TYPEDEFHPP__
#define __TYPEDEFHPP__

#include <vector>
#include <functional>

namespace NET
{
	typedef struct tagFileEvent {
		int fd;
		int mask;
		char* data;
		unsigned int dataSize;
		::std::function<int(struct tagFileEvent*)> readProc;
		::std::function<int(struct tagFileEvent*)> writeProc;
		
		void clean() {
			if ( NULL != data ) {
				delete data;
				data = NULL;
			}
			dataSize = 0;
		}
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
