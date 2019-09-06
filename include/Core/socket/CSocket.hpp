#ifndef __NET_CSOCKET_HPP__
#define __NET_CSOCKET_HPP__

namespace NET
{
    class CSocket : public CObject
    {
    public:// TODO: define const here
        #define NET_NONE                            0x00
        #define NET_READABLE                        0x01
        #define NET_WRITEABLE                       0x02

    private:// TODO: define const here

    public:// TODO: define embed class or struct or enum here

    private:// TODO: define embed class or struct or enum here

    public:// TODO: define your public method here
        virtual void 						init();
        virtual void 						destroy();
        
        void 								setKeepAlive( BOOLEAN, INT );
        void 								setTimeOut( INT );

        void    							setReusePort( BOOLEAN );
        void 								setReuseAddress( BOOLEAN );
        void								setNonBlock( BOOLEAN );

        /**
        * set operator after close socket
        * l_onoff:
        *   on)     default, response immediately and transfer remain data
        */  off)    
        void								setLinger( BOOLEAN, INT );
        void 								setNoDelay( BOOLEAN );

        void								setSendBuffSize( INT64 );
        void 								setRecvBuffSize( INT64 );	

    protected:// TODO: define your protected method here
        CSocket();
        virtual ~CSocket();

        virtual UINT						read();
        virtual UINT						write();	

        CSocket( CSocket& ) = delete;
        CSocket( const CSocket& ) = delete;

    private:// TODO: define your private method here
        
    protected:// property
        INT                             m_fd;
        BOOLEAN                         m_bIsOpenning;

    private:// property

    };
}

#endif  // __NET_CSOCKET_HPP__