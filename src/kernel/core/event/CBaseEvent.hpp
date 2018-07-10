#ifndef __CBASEEVENTHPP__
#define __CBASEEVENTHPP__

namespace NET
{
    class CBaseEvent
    {
        public: 
            CBaseEvent();
            virtual ~CBaseEvent();

            UINT        priority()                      { return m_uiPriority; }
            void        setPriority(UINT uiPriority)    { m_uiPriority = uiPriority; }

            UINT        fd()                            { return m_fd; }
            void        setFd(UINT fd)                  { m_fd = fd; }
           
        protected:   
            UINT        m_fd;
            UINT        m_uiPriority;     
    };
} //NET
#endif