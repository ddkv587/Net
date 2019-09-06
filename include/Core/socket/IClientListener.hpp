#ifndef __NET_ICLIENTLISTENER_HPP__
#define __NET_ICLIENTLISTENER_HPP__

//interface for the body which loop the client after accept form listener
namespace NET
{
    class IClientListener
    {
    public:// TODO: define your public method here
        virtual INT         		addClient( INT ) = 0;
        virtual void        		delClient( INT ) = 0;

        virtual BOOLEAN     		isEnable() = 0;
        virtual BOOLEAN     		isFull() = 0;
    
        virtual UINT        		size() const = 0;
    };
}
#endif  // __NET_ICLIENTLISTENER_HPP__
