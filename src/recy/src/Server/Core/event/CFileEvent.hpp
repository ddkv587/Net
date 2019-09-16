#ifndef __CFILEEVENTHPP__
#define __CFILEEVENTHPP__

namespace NET
{
    class CFileEvent : public CBaseEvent
    {
        public:
            #define EVENT_BLOCK_SIZE    1024

            enum ETriggerType {
                TT_LEVEL = 0,
                TT_EDGE,
            }
            
        public: 
            CFileEvent();
            virtual ~CFileEvent();

            virtual BOOLEAN         readProc();
            virtual BOOLEAN         writeProc();

            void                    clear();
            void                    release();
            UINT                    resize(UINT uiSize);

            BOOLEAN                 isEmpty()                   { return m_uiSize == 0; }
            UINT                    size()                      { return m_uiSize; }
            UINT                    point()                     { return m_uiPoint; }
            void*&                  buffer()                    { return m_dataBuffer; }

            void                    setNonBlock();
  
        private: 
            UINT                    m_uiSize;   
            UINT                    m_uiPoint;
            void*                   m_dataBuffer;

            ETriggerType            m_eTriggerType;
    };
} //NET
#endif