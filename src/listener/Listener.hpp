#ifndef __LISTENERHPP__
#define __LISTENERHPP__

namespace NET
{
	class CListener : public CFileEvent 
	{
		public:
			CListener();
			virtual ~CListener();

			virtual BOOLEAN         					readProc();
            virtual BOOLEAN        				 		writeProc();

			virtual void 	                    		addListener(IClientListener*);
			virtual void 	                    		delListener(IClientListener*);
			void 			                    		printListener();
        
            const CSocketServer*                		server() const                          { return m_pServer; }

		protected:
			virtual IClientListener*            		balance();
            const ::std::list<IClientListener*>&      	listener() const                        { return m_lstListener; }
			
			void mainLoop(void* arg) override;

		private:
			void setNonBlock(int);

		private:
			CSocketServer* 				        m_pServer;
			::std::list<IClientListener*>       m_lstListener;
	};
}
#endif
