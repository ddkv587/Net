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
            void            	reload();

			void 				start(void* arg = nullptr);

		protected:
			CMain();
			~CMain();

		private:
			CMain(CMain&) = delete;
			CMain(const CMain&) = delete;

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
