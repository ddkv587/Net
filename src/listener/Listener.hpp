#ifndef __LISTENERHPP__
#define __LISTENERHPP__

namespace NET
{
	class CListener : public CThreadBase 
	{
		public:
			CListener();
			virtual ~CListener();

			virtual void 	            addFileListener(IFileListener*);
			virtual void 	            delFileListener(IFileListener*);

			void 			            printListener();
        
            CSocketServer*              getSocketServer() const                 { return m_pServer; }

		protected:
			virtual IFileListener*      balance(::std::list<IFileListener*>&);
			
			void mainLoop(void* arg);

		private:
			void setNonBlock(int);

		private:
			CSocketServer* 				m_pServer;
			::std::list<IFileListener*> m_lstListener;
	};
}
#endif
