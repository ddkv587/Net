#ifndef __LISTENERHPP__
#define __LISTENERHPP__

#include "Processer.hpp"

class IListener
{
	virtual int addEvent(int, int) = 0;
	virtual int delEvent(int, int) = 0;
};

class CListener
{
public:
	CListener();
	virtual ~CListener();
	
	void run();
	void* mainLoop(void*);

private:

private:
	int socket_fd;
	int m_iPosi;
	thread_info m_thread;
	CProcesser process[4]; 
};
#endif
