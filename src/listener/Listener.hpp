#ifndef __LISTENERHPP__
#define __LISTENERHPP__

namespace NET
{
	class CListener : public ThreadBase 
	{
		public:
			CListener();
			virtual ~CListener();

			virtual void 	addFileListener(IFileListener*);
			virtual void 	delFileListener(IFileListener*); 

			void 			printListener();

		protected:
			virtual IFileListener* scheduling(::std::list<IFileListener*>&);
			
			void mainLoop(void* arg);

		private:
			void setNonBlock(int);

		private:
			CSocketServer 				m_server;
			::std::list<IFileListener*> m_lstListener;
	};
}
#endif
