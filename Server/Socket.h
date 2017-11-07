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

		void init();

		void start();
		void stop();

		void pend();
		void block();


	private:
				
		unsigned int m_uiPort;
}

#endif
