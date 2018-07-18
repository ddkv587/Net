#ifndef __CCModuleManagerHPP__
#define __CCModuleManagerHPP__

namespace NET
{
    class CModuleManager final
    {
        public: 
            static CModuleManager*		getInstance();

            void                        registerModule(CBaseModule*);          

        protected:
            CModuleManager();
            virtual ~CModuleManager();    
            CModuleManager(CModuleManager&) = delete;
			CModuleManager(const CModuleManager&) = delete; 
           
        protected:   
            static CModuleManager* 	            s_pInstance;
    };
} //NET
#endif