#ifndef __CCModuleManagerHPP__
#define __CCModuleManagerHPP__

namespace NET
{
    class CModuleManager final
    {
        public: 
            static CModuleManager*		getInstance();
            
            void                        registerModule(CBaseModule*); 
            void                        removeModule(STRING);   
            void                        removeModule(CBaseModule*); 

            BOOLEAN                     isExist(STRING); 
            BOOLEAN                     isExist(CBaseModule*);
            CBaseModule*                getModule(STRING);           

        protected:
            CModuleManager();
            ~CModuleManager();    
            CModuleManager(CModuleManager&) = delete;
			CModuleManager(const CModuleManager&) = delete;
           
        protected:   
            static CModuleManager* 	         s_pInstance;

            SMUTEX                           m_mutex;
            SMAP<STRING, CBaseModule*>       m_mapModules;
    };
} //NET
#endif