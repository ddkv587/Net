#ifndef __SOCKETSERVERHPP__
#define __SOCKETSERVERHPP__

namespace NET
{
	class CSocketServer : public CSocket
	{
		public:
			CSocketServer();
			virtual ~CSocketServer();
	
			void init();
			void destroy();

			void setPort(int iPort) 	{ m_port = iPort; }
			int  getPort() const		{ return m_port; }

			void setReuseAddr(bool on);

			CSocketServer(CSocketServer&) = delete;
			CSocketServer(const CSocketServer&) = delete;

		private:
			int m_port;
	};
}

#endif
