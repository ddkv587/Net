#ifndef __CONFIGPARSERHPP__
#define __CONFIGPARSERHPP__

namespace NET
{
	class ConfigParser final
	{
		public:
			struct tagSystemInfo {
				UINT    uiPriority;
				UINT    uiThreadCount;
			};
        
            struct tagSocketInfo {
                UINT        uiPort;
                
                BOOLEAN     bKeepAlive;
                UINT        uiAliveValue;
                UINT        uiTimeOut;
                
                BOOLEAN     bReusePort;
                BOOLEAN     bReuseAddress;
            };

		public:
			static ConfigParser* 		        getInstance();

			BOOLEAN						        initialize();
			inline const tagSystemInfo&		    getSystemInfo()	const	    { return m_tagSysInfo; }
            inline const tagSocketInfo&         getSocketInfo() const       { return m_tagSocketInfo; }
		
		protected:
			ConfigParser();
			~ConfigParser();

		private:
            void                        preset();
            BOOLEAN                     load();
            BOOLEAN                     save();
        
			void		                buildSystemInfo(const XMLElement*);
            void                        buildSocketInfo(const XMLElement*);
	
		private:
			static ConfigParser*        m_pInstance;

			tagSystemInfo	            m_tagSysInfo;
            tagSocketInfo               m_tagSocketInfo;
        
            XMLHelper*                  m_xmlHelper;
	};	
}
#endif
