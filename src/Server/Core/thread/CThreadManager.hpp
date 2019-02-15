#ifndef __CThreadManagerHPP__
#define __CThreadManagerHPP__

namespace NET
{
    class CThreadManager final
    {
        public: 
            static CThreadManager*		getInstance();

            BOOLEAN                     initialize();
            void                        unInitialize();

        protected:
            CThreadManager();
            virtual ~CThreadManager();    
            CThreadManager(CThreadManager&) = delete;
			CThreadManager(const CThreadManager&) = delete; 
           
        protected:   
            static CThreadManager* 	            s_pInstance;
    };
} //NET
#endif