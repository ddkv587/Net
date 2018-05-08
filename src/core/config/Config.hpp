#ifndef __CONFIGHPP__
#define __CONFIGHPP__

namespace NET
{
	class CConfig final
	{
		public:
			struct tagSystemInfo
			{
				unsigned int m_uiPriority;
				unsigned int m_uiThreadCount;
			};

		public:
			static CConfig* 		getInstance();

			void					initialize();
			inline tagSystemInfo&	getSystemInfo()	const	{ return m_tagSysInfo; }
		
		protected:
			CConfig();
			~CConfig();

		private:
			void		buildSystenInfo();
	
		private:
			static CConfig* m_pInstance;

			tagSystemInfo	m_tagSysInfo;
	};	
}
#endif
