#ifndef __MULTIKQUEUEHPP__
#define __MULTIKQUEUEHPP__

namespace NET
{
    class CMultiKqueue : public IMultiBase
    {
        public:
            CMultiKqueue();
            virtual ~CMultiKqueue();
        
			void 			init();
			void			destroy();
            INT             setSize(INT);
        
            INT             addFileEvent(INT, INT, EVENT_LOOP*);
            void            delFileEvent(INT, INT, EVENT_LOOP*);
            INT             eventLoop(void*, EVENT_LOOP*);   // struct timeval
        
        private:
            INT             m_kqfd;
            struct kevent*  m_events;
    };
}

#endif
