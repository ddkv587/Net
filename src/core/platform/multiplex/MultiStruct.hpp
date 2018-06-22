#ifndef __MULTISTRUCTHPP__
#define __MULTISTRUCTHPP__

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

	struct tagData;
	struct tagFileEvent;
	struct tagFiredEvent;
	struct tagTimeEvent;
	struct tagEventLoop;

	typedef ::std::function<INT(const struct tagEventLoop* loop, INT fd, struct tagData*& dataBuff, INT mask)> fileProc;
	typedef ::std::function<INT(const struct tagEventLoop* loop, INT id, INT repeat)> timeProc;

	typedef struct tagData
	{
		UINT 			uiSize;
		void*			buffer;
	} DATA;

	typedef struct tagFileEvent {
		INT 			mask;
		DATA*			clientData;
		fileProc 		readProc;
		fileProc 		writeProc;		
	} FILE_EVENT;

	typedef struct tagFiredEvent {
		INT 			index;
		INT 			mask;
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
		UINT     					uiSize;
		::std::vector<TIME_EVENT>   lstTimeEvent;
		::std::vector<FILE_EVENT>   lstFileEvent;
	} EVENT_LOOP;
}
#endif
