#ifndef __TYPEDEFHPP__
#define __TYPEDEFHPP__

namespace NET
{
	typedef struct tagFileEvent {
		int mask;
		void* data;
		unsigned int dataSize;
		std::function<int(int, void*, int)> readProc;
		std::function<int(int, void*, int)> writeProc;
	} FILE_EVENT;

	typedef struct tagTimeEvent {
		long long id;
		long sec;
		long ms;
	}TIME_EVENT;

	typedef struct tagFiredEvent {
		int fd;
		int mask;
	} FIRED_EVENT;

	typedef struct tagEventLoop {
		int maxfd;
		int size;
		FILE_EVENT *event;
		FIRED_EVENT *fired;
		TIME_EVENT *timeEvent;
		void* arg;
	} EVENT_LOOP;
}
#endif
