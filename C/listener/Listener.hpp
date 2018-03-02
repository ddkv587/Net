#ifndef __LISTENERHPP__
#define __LISTENERHPP__

#include <list>
#include "ThreadBase.hpp"

namespace NET
{
	class IFileListener
	{
		public:
			virtual int addFileEvent(int, int) = 0;
			virtual void delFileEvent(int, int) = 0;
	};

	class CListener : public ThreadBase 
	{
		public:
			CListener();
			virtual ~CListener();

			void addFileListener(IFileListener*);
			void delFileListener(IFileListener*); 

			const int getSocketFD() { return m_socketFD; }

		protected:
			void mainLoop(void* arg);

		private:
			void setNonBlock(int);

		private:
			int m_socketFD;
			std::list<IFileListener*> m_lstListener;
	};
}
#endif
