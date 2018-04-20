#ifndef __SOCKETHPP__
#define __SOCKETHPP__

namespace NET
{
	class CSocket
	{
		public:
			void 	init();
			void 	destroy();
			
			void 	setKeepAlive(bool, int);
			void 	setTimeOut(int);

			void 	setReuseAddr(bool);
			void 	setReusePort(bool);

			void	setSendBuffSize(int);
			void 	setSendTimeOut(int);

			void	setRecvBuffSize(int);
			void 	setRecvTimeOut(int);

			int 	getSocketFD()		 	{ return m_fd; }

		protected:
			CSocket();
			virtual ~CSocket();

			CSocket(CSocket&) = delete;
			CSocket(const CSocket&) = delete;

		protected:
			int 	m_fd;
	};
}

#endif
