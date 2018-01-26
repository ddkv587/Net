#ifndef __TYPEDEFHPP__
#define __TYPEDEFHPP__

namespace NET
{
	typedef struct tagFileEvent {
		int mask;
		std::function<void(int, void*, int)> readProc;
		std::function<void(int, void*, int)> writeProc;
		void* data;
		int dataSize;
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
