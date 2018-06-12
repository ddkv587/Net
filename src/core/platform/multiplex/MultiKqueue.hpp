#ifndef __MULTIKQUEUEHPP__
#define __MULTIKQUEUEHPP__

namespace NET
{
    class CMultiKqueue : public CMultiBase
    {
        public:
            CMultiKqueue();
            virtual ~CMultiKqueue();
        
			void 			init();
			void			destroy();
            INT             setSize(INT);
        
            INT             addFileEvent(INT, INT);
            void            delFileEvent(INT, INT);
            INT             eventLoop(void*);   // struct timeval
        
        private:
            INT             m_kqfd;
            struct kevent*  m_events;
    };
}

#endif
