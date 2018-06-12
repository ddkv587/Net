#ifndef __IFILELISTENERHPP__
#define __IFILELISTENERHPP__

//interface for the body which loop the client after accept form listener
class IClientListener
{
	public:
		virtual INT         addClient(INT) = 0;
		virtual void        delClient(INT) = 0;

        virtual BOOLEAN     isEnable() = 0;
        virtual BOOLEAN     isFull() = 0;
    
		virtual UINT        size() const = 0;
};
#endif
