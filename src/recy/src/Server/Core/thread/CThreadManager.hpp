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

            static ::std::tuple<INT, INT> CThread::range(EPolicy policy)
            {
                return ::std::make_tuple( sched_get_priority_max(transformPolicy(policy)), sched_get_priority_min(transformPolicy(policy)));
            }

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