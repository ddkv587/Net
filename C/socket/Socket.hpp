#ifndef __SOCKETHPP__
#define __SOCKETHPP__

namespace NET
{
	class CSocket
	{
		public:
			void 	setKeepAlive(bool, int);
			void 	setTimeOut(int);

			void 	setReuseAddr(bool);
			void 	setReusePort(bool);

			int 	getSocketFD()	const 		{ return m_fd; }

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
