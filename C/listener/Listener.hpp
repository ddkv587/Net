#ifndef __LISTENERHPP__
#define __LISTENERHPP__

#include <list>
#include "ThreadBase.hpp"

class IFileListener
{
	virtual int addFileEvent(int, int) = 0;
	virtual void delFileEvent(int, int) = 0;
};

class CListener : public ThreadBase 
{
public:
	CListener();
	virtual ~CListener();

	void addFileListener(const IFileListener*);
	void delFileListener(const IFileListener*); 

	const int getSocketFD() { return m_socketFD; }

protected:
	void mainLoop();

private:
	void setNonBlock(int);

private:
	int m_socketFD;
	std::list<IFileListener*> m_lstListener;
};
#endif
