#ifndef __NET_CBASEMESSAGE_HPP__
#define __NET_CBASEMESSAGE_HPP__

namespace NET
{
    class CBaseMessage : public CObject
    {
    public:// TODO: define const here
        enum EMessageType
        {
            MT_UNKNOW = -1,
            MT_SYSTEM,
            MT_USER,
            MT_TIMER
        };

    private:// TODO: define const here

    public:// TODO: define embed class or struct or enum here
        virtual void            handleMessage( ) = 0;

    private:// TODO: define embed class or struct or enum here

    public:// TODO: define your public method here
        EMessageType            type() const            { return m_eType; }

    protected:// TODO: define your protected method here
        CBaseMessage() : m_eType( MT_UNKNOW )           { }
        virtual ~CBaseMessage()                         { }

    private:// TODO: define your private method here
        
    protected:// property
        EMessageType        m_eType;

    private:// property
        
    };
}

#endif  // __NET_CBASEMESSAGE_HPP__
