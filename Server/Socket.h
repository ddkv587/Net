#ifndef __SOCKET_H__
#define __SOCKET_H__

class SocketBase
{
	
};

class SocketServer
{
	public:
		SocketServer(unsigned int port = 8001);
		virtual ~SocketServer();

		int getSocketfd() { return m_iSocketfd; }

	private:
		int 		m_iSocketfd;			
		unsigned int m_uiPort;
}

#endif
