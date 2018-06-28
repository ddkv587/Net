#ifndef __CBASEEVENTHPP__
#define __CBASEEVENTHPP__

namespace NET
{
    class CBaseEvent
    {
        public: 
        	typedef ::std::function<INT(const struct tagEventLoop* loop, INT fd, struct tagData*& dataBuff, INT mask)> fileProc;
	        typedef ::std::function<INT(const struct tagEventLoop* loop, INT id, INT repeat)> timeProc;

        private:         
    };
} //NET
#endif