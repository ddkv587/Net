#ifndef __CMAINHPP__
#define __CMAINHPP__

namespace NET
{
    class CMain final
    {
        public:

        public:
            static CMain*		getInstance();

            BOOLEAN				initialize();
            void 				unInitialize();

//================= life circle ==========================
            void            	start();
            void				stop();
            void				pause();
            void				resume();
            void				restart();

//================= module =======================
            BOOLEAN				addModule();
            void				delModule();
            void				reloadModule();
        
        protected:
            CMain();
            ~CMain();
            CMain(CMain&) = delete;
            CMain(const CMain&) = delete;

        private:		
            BOOLEAN         	innerInitSystem();
            BOOLEAN         	innerInitListener();
            BOOLEAN         	innerInitProcessor();
            BOOLEAN         	innerInitUpdate();
        
            void            	innerDestroyListener();
            void            	innerDestroyProcessor();
            void           	 	innerDestroyUpdate();
        
        private:
            static CMain* 	            		s_pInstance;
            ::std::List<>

            BOOLEAN			            		m_bInitialized;
    };
}
#endif
