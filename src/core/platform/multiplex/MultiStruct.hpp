#ifndef __TYPEDEFHPP__
#define __TYPEDEFHPP__

#include <vector>
#include <functional>

namespace NET
{
	enum EVENT_TYPE
	{
		ET_NONE = 0,
		ET_FILE,
		ET_TIME,
		ET_MAX,
	};

	typedef ::std::function<INT(INT fd, void* dataBuff, INT mask)> fileProc;
	typedef ::std::function<INT(INT id, INT repeat)> timeProc;

	typedef struct tagFileEvent {
		int fd;
		int mask;
		fileProc* readProc;
		fileProc* writeProc;
	} FILE_EVENT;

	typedef struct tagFiredEvent {
		int 			index;
		int 			mask;
		EVENT_TYPE		type;
	} FIRED_EVENT;

	typedef struct tagTimeEvent {
		INT				id;
		UINT 			uiValue;
		UINT 			uiInterval;
		INT  			iRepeat;
		timeProc*		handle;
	} TIME_EVENT;

	typedef struct tagEventLoop {
		INT     maxfd;
		INT     size;
        void*   arg;
		::std::vector<TIME_EVENT>   lstTimeEvent;
		::std::vector<FILE_EVENT>   lstFileEvent;
		::std::vector<FIRED_EVENT>  lstFired;
	} EVENT_LOOP;
}
#endif
