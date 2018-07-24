#ifndef __CBASEMODULEHPP__
#define __CBASEMODULEHPP__

namespace NET
{
    class CBaseModule
    {
        public: 
            virtual BOOLEAN         load()          = 0;
            virtual void            remove()        = 0;

/*=====================life circle==========================
            virtual void            start()         = 0;
			virtual void			stop()          = 0;
            virtual void			pause()         = 0;
			virtual void			resume()        = 0;
			virtual void			restart()       = 0;
*/
            CBaseModule() : m_bIsInitialize(FALSE) {}
            virtual ~CBaseModule() {}

            BOOLEAN                 isInitialize()          { return m_bIsInitialize; }
            BOOLEAN                 isExclusive()           { return m_bIsExclusive; }

        protected:
            CBaseModule(CBaseModule&) = delete;
			CBaseModule(const CBaseModule&) = delete;

        private:
            BOOLEAN                 m_bIsInitialize;

//====================== module info =============================         
            STRING                  m_strName;

//====================== priority =============================
            BOOLEAN                 m_bIsExclusive;
            UINT                    m_uiPriority;

//====================== out ============================
            UINT                    m_uiDataLimit;
            std::queue<void*>       m_dataQueue;

//====================== in ============================
            CBaseModule*            m_pAttachModule;
    };
} //NET
#endif