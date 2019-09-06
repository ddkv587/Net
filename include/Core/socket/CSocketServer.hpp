#ifndef __NET_CSOCKETSERVER_HPP__
#define __NET_CSOCKETSERVER_HPP__

namespace NET
{
    class CSocketServer : public CSocket
    {
    public:// TODO: define const here

    private:// TODO: define const here

    public:// TODO: define embed class or struct or enum here

    private:// TODO: define embed class or struct or enum here

    public:// TODO: define your public method here
        CSocketServer( STRING ip = STRING_NULL, UINT port = 8000 );
        virtual ~CSocketServer();

        virtual void	                init();
        virtual void 	                destroy();

        void 			                setIP(const STRING& ip)		        { m_strIP = ip; }
        const STRING& 	                ip() const 			    	        { return m_strIP; };

        void 			                setPort(UINT iPort) 				{ m_uiPort = iPort; }
        UINT  			                port() const						{ return m_uiPort; }
        
        BOOLEAN			                bindAndListen();

    protected:// TODO: define your protected method here
        CSocketServer(CSocketServer&) = delete;
        CSocketServer(const CSocketServer&) = delete;

    private:// TODO: define your private method here
        
    protected:// property

    private:// property
        STRING 	                m_strIP;
        UINT 	                m_uiPort;
    };
}

#endif  // __NET_CSOCKETSERVER_HPP__