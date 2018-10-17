#ifndef __SOCKETHPP__
#define __SOCKETHPP__

namespace NET
{
#define NET_NONE                            0x00
#define NET_READABLE                        0x01
#define NET_WRITEABLE                       0x02

	class CSocket
	{
		public:
			class SocketException : public std::exception
			{
				public:
					SocketException(INT errno) : m_errno(errno) {}
					const char* what() const noexcept
					{
						return strerror(m_errno);
					}

				private:
					INT m_errno;
			}

		public:
			virtual void 						init() noexcept;
			virtual void 						destroy() noexcept;
			
			void 								setKeepAlive(BOOLEAN, INT) noexcept;
			void 								setTimeOut(INT) noexcept;

            void    							setReusePort(BOOLEAN) noexcept;
			void 								setReuseAddress(BOOLEAN) noexcept;
			void								setNonBlock(BOOLEAN) noexcept;
			void								setLinger(BOOLEAN, INT) noexcept;
			void 								setNoDelay(BOOLEAN) noexcept;

			void								setSendBuffSize(INT64) noexcept;
			void 								setRecvBuffSize(INT64) noexcept;			

			friend inline const CSocket&		operator>>(const CSocket&, const CBuffer* const pBuffer);		//send
			friend inline const CSocket&		operator<<(const CSocket&, CBuffer* const pBuffer);				//recive

		protected:
			CSocket();
			virtual ~CSocket();

			virtual UINT						read();
			virtual UINT						write();	

			CSocket(CSocket&) = delete;
			CSocket(const CSocket&) = delete;

		protected:
			INT 			m_fd;
			BOOLEAN			m_bIsOpenning;
	};
}

#endif
