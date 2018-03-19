#ifndef __LISTENERHPP__
#define __LISTENERHPP__

namespace NET
{
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
			::std::list<IFileListener*> m_lstListener;
	};
}
#endif
