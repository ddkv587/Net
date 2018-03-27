#ifndef __SOCKETHPP__
#define __SOCKETHPP__

namespace NET
{
	class CSocket
	{
		public:
			void setKeepAlive(bool, int, int, int);
			void setTimeOut(int);

		protected:
			CSocket();
			virtual ~CSocket();

			CSocket(CSocket&) = delete;
			CSocket(const CSocket&) = delete;

		private:
			int fd;
	};
}

#endif
