#ifndef __SOCKETSERVERHPP__
#define __SOCKETSERVERHPP__

namespace NET
{
	class CSocketServer : public CSocket
	{
		public:
			CSocketServer();
			virtual ~CSocketServer();
	
			void 	init();
			void 	destroy();

			void 			setServerIP(char* ip)	{ strncpy(m_serverIp, ip, 16); }
			const char* 	getServerIP() const 	{ return m_serverIp; };

			void 	setPort(int iPort) 		{ m_serverPort = iPort; }
			int  	port() const			{ return m_serverPort; }
			
			void	bindAndListen();

			CSocketServer(CSocketServer&) = delete;
			CSocketServer(const CSocketServer&) = delete;

		private:
			char 	m_serverIp[16];		//max is 255.255.255.255
			int 	m_serverPort;
	};
}

#endif
