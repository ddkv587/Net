#ifndef __LISTENERHPP__
#define __LISTENERHPP__

namespace NET
{
	class CListener : public CThreadBase 
	{
		public:
			CListener();
			virtual ~CListener();

			virtual void 	                    addListener(IFileListener*);
			virtual void 	                    delListener(IFileListener*);
			void 			                    printListener();
        
            const CSocketServer*                server() const                          { return m_pServer; }

		protected:
			virtual IClientListener*            balance();
            ::std::list<IClientListener*>&      listener() const                        { return m_lstListener; }
			
			void mainLoop(void* arg);

		private:
			void setNonBlock(int);

		private:
			CSocketServer* 				        m_pServer;
			::std::list<IClientListener*>       m_lstListener;
	};
}
#endif
