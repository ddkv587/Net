#ifndef __MULTIKQUEUEHPP__
#define __MULTIKQUEUEHPP__

namespace NET
{
    class CMultiKqueue : CMultiBase
    {
        public:
            CMultiKqueue();
            virtual ~CMultiKqueue();
        
            int             setSize(int size);
        
            int             addFileEvent(int, int);
            void            delFileEvent(int, int);
            int             eventLoop(void* timeout = NULL); // struct timeval
        
        private:
            int             m_kqfd;
            struct kevent*  m_events;
    };
}

#endif
