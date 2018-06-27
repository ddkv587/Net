#ifndef __CFILEEVENTHPP__
#define __CFILEEVENTHPP__

namespace NET
{
    class CFileEvent : public CBaseEvent
    {
        public: 
            virtual void            readProc() = 0;
            virtual void            writeProc() = 0;

            BOOLEAN                 isEmpty();
            void                    clearBuff();
            UINT                    resize();
            const void*             buffer()            { return m_dataBuffer; }
  
        private: 
            UINT            m_fd;

            UINT            m_bufferSize;
            void*           m_dataBuffer;
    };
} //NET
#endif