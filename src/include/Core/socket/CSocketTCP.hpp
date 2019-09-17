#ifndef __NET_CSOCKETTCP_HPP__
#define __NET_CSOCKETTCP_HPP__

namespace NET
{
    class CSocketTCP : public CSocketBase
    {
    public:// TODO: defin const here

    private:// TODO: define const here

    public:// TODO: define embed class or struct or enum here

    private:// TODO: define embed class or struct or enum here

    public:// TODO: define your public method here
        virtual void                    init();
        virtual void                    destroy();

        void                            ip( const STRING& ip )                  { m_strIP = ip; }
        const STRING&                   ip() const                              { return m_strIP; };

        void                            port( UINT iPort )                      { m_uiPort = iPort; }
        UINT                            port() const                            { return m_uiPort; }

    protected:// TODO: define your protected method here
        CSocketTCP( STRING ip = STRING_NULL, UINT port = 8000 );
        virtual ~CSocketTCP();

        CSocketTCP( CSocketTCP& ) = delete;
        CSocketTCP( const CSocketTCP& ) = delete;

    private:// TODO: define your private method here
        
    protected:// property

    private:// property
        STRING                      m_strIP;
        UINT                        m_uiPort;
    };
}

#endif  // __NET_CSOCKETTCP_HPP__