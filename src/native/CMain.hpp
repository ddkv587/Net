#ifndef __CMAINHPP__
#define __CMAINHPP__

namespace NET
{
	class CMain : public CThreadBase
	{
		public:
			struct tagSystemInfo
			{
				UINT m_uiPriority;
				UINT m_uiThreadCount;
				UINT m_uiShortTurnLimit;
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
			static CMain* 	            		s_pInstance;

            CUpdate*                   		 	m_pUpdate;
            CListener*                  		m_pListener;
            ::std::forward_list<CProcessor*>    m_lstShortTurn;
            ::std::forward_list<CProcessor*>    m_lstLongTurn;
			bool			            		m_bInitialized;
	};
}
#endif
