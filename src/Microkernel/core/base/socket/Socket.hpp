#ifndef __SOCKETHPP__
#define __SOCKETHPP__

namespace NET
{
	class CSocket
	{
		public:
			virtual void 	init() noexcept;
			virtual void 	destroy() noexcept;
			
			void 			setKeepAlive(BOOLEAN, INT) noexcept;
			void 			setTimeOut(INT) noexcept;

            void    		setReusePort(BOOLEAN) noexcept;
			void 			setReuseAddress(BOOLEAN) noexcept;
			void			setNonBlock(BOOLEAN) noexcept;
			void			setLinger(BOOLEAN, INT) noexcept;
			void 			setNoDelay(BOOLEAN) noexcept;

			void			setSendBuffSize(INT64) noexcept;
			void 			setRecvBuffSize(INT64) noexcept;
			
			inline INT 		getSocketFD()				{ return m_fd; }

		protected:
			CSocket();
			virtual ~CSocket();

			CSocket(CSocket&) = delete;
			CSocket(const CSocket&) = delete;

		protected:
			INT 			m_fd;
			BOOLEAN			m_bIsOpenning;
	};
}

#endif
