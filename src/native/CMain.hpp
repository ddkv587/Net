#ifndef __CMAINHPP__
#define __CMAINHPP__

namespace NET
{
	class CMain : public CThreadBase
	{
		public:
			struct tagSystemInfo
			{
				unsigned int m_uiPriority;
				unsigned int m_uiThreadCount;
			};

		public:
			static CMain*	getInstance();

			bool			initialize();
			void 			unInitialize();
            void            reload();

		protected:
			CMain();
			virtual ~CMain();
			void 			mainLoop(void* arg);

		private:
			CMain(CMain&) = delete;
			CMain(const CMain&) = delete;
        
            BOOLEAN         innerInitListener();
            BOOLEAN         innerInitProcessor();
            BOOLEAN         innerInitUpdate();
        
            void            innerDestroyListener();
            void            innerDestroyProcessor();
            void            innerDestroyUpdate();
        
		private:
			static CMain* 	            s_pInstance;

            CListener*                  m_pListener;
            CUpdate*                    m_pUpdate;
            ::std::list<CProcessor*>    m_lstProcessor;
			bool			            m_bInitialized;
	};
}
#endif
