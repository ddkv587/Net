#ifndef __MODULEHPP__
#define __MODULEHPP__

namespace NET
{
	class CModule
	{
		public:
			struct tagModuleConfig
			{
				int 					iPriority;
				::std::string 			strName;		
				::std::string 			strPath;		
				::std::string 			strConfig;	
				::std::string			strDepends;
			};

		public:
			virtual bool 		load() = 0;
			virtual void 		remove() = 0;
			virtual bool 		initialize() = 0;
			virtual void 		unInitialize() = 0;
			bool		 		isInitialized() 					{ return m_bInitialized; }

			void				setPriority(int pri)				{ m_config.iPriority = pri; }
			void				setName(::std::string name)			{ m_config.strName = name; }
			void				setPath(::std::string path)			{ m_config.strPath = path; }
			void				setConfig(::std::string config)		{ m_config.strConfig = config; }
			void				setConfig(::std::string depends)	{ m_config.strDepends = depends; }
			tagModuleConfig&	getConfig() const 					{ return m_config; }

		protected:
			CModule() : m_bInitialized(false) {}
			virtual ~CModule() {}

		protected:
			bool					m_bInitialized;
			tagModuleConfig 		m_config;
	};
}
#endif
