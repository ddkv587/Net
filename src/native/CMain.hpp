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

			bool			loadConfig();
			void			saveConfig();

			bool			initialize();
			void 			unInitialize();

			void			reload();

		protected:
			CMain();
			virtual ~CMain();
			void 			mainLoop(void* arg);

		private:
			CMain(CMain&) = delete;
			CMain(const CMain&) = delete;

			void 			innerInitModule();
			void 			innerRemoveModule();

			void			innerInitProcessor();
			void			innerInitListener();

			void			buildSystemInfo();

		private:
			static CMain* 	s_pInstance;

			bool			m_bInitialized;	
			::std::forward_list<CModule> m_lstModule;
	};
}
#endif
