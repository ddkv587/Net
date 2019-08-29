#ifndef __SOCKETSERVERHPP__
#define __SOCKETSERVERHPP__

namespace NET
{
	class CSocketServer : public CSocket
	{
		public:
			CSocketServer(STRING ip = STRING_NULL, UINT port = 8000);
			virtual ~CSocketServer();

			virtual void	init() noexcept;
			virtual void 	destroy() noexcept;

            void 			setIP(const STRING& ip)		        { m_strIP = ip; }
			const STRING& 	ip() const 			    	        { return m_strIP; };

			void 			setPort(UINT iPort) 				{ m_uiPort = iPort; }
			UINT  			port() const						{ return m_uiPort; }
			
			BOOLEAN			bindAndListen();

			CSocketServer(CSocketServer&) = delete;
			CSocketServer(const CSocketServer&) = delete;

		private:
			STRING 	    m_strIP;
			UINT 	    m_uiPort;
	};
}
#endif
