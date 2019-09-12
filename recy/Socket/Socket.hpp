#ifndef __SOCKET_H__
#define __SOCKET_H__

namespace Net
{
    class CSocket
    {
        public:
            CSocket();
            CSocket(const CSocket&) = delete;
            CSocket(CSocket&&);

            virtual ~CSocket();

            virtual int destroy();

            const int getSocketFd() { return m_socketfd; }
            int setSocketOpt(int level, int optname, const char* optval, socketlen_t optlen) const;    

        protected:
            int     m_socketfd;
    };
}
#endif
