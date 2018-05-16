#ifndef __CMAINHPP__
#define __CMAINHPP__

namespace NET
{
	class CMain final
	{
		public:

		public:
			static CMain*	getInstance();

			BOOLEAN			initialize();
			void 			unInitialize();
            void            reload();

			void 			start(void* arg = NULL);

		protected:
			CMain();
			~CMain();

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
            ::std::list<CProcessor*>    		m_lstShortTurn;
            ::std::list<CProcessor*>    		m_lstLongTurn;
			BOOLEAN			            		m_bInitialized;
	};
}
#endif
