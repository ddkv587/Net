#ifndef __CFILEEVENTHPP__
#define __CFILEEVENTHPP__

namespace NET
{
    class CFileEvent : public CBaseEvent
    {
        public: 
            virtual void            readProc() = 0;
            virtual void            writeProc() = 0;

            BOOLEAN                 isEmpty()               { return m_bufferSize == 0; }
            void                    release();
             void                   clear();
            UINT                    resize(UINT uiSize);
            void*&                  buffer()                { return m_dataBuffer; }
  
        private: 
            UINT                    m_fd;

            UINT                    m_bufferSize;
            void*                   m_dataBuffer;
    };
} //NET
#endif