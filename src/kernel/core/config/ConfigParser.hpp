#ifndef __CONFIGPARSERHPP__
#define __CONFIGPARSERHPP__

namespace NET {
    class ConfigParser final 
    {
    public:
        enum ESYSTEM_ROLE
        {
            SR_NONE = 0,
            SR_MASTER,
            SR_SLAVE,
            SR_MAX
        };

        struct tagSystemInfo {
            UINT            uiPriority;
            UINT            uiThreadCount;
            UINT            uiShortTurnLimit;

            UINT            uiMaxFileSize;
            UINT            uiLimitOpenSize;

            ESYSTEM_ROLE    eRole;
        };

        struct tagSocketInfo {
            UINT uiPort;

            BOOLEAN bKeepAlive;
            UINT uiAliveValue;
            UINT uiTimeOut;

            BOOLEAN bReusePort;
            BOOLEAN bReuseAddress;
        };

    public:
        static ConfigParser* getInstance();

        BOOLEAN initialize();

        inline const tagSystemInfo& getSystemInfo() const {
            return m_tagSysInfo;
        }

        inline const tagSocketInfo& getSocketInfo() const {
            return m_tagSocketInfo;
        }

    protected:
        ConfigParser();
        ~ConfigParser();

    private:
        void preset();
        BOOLEAN load();
        BOOLEAN save();

        void loadSystemInfo(const XMLElement*);
        void loadSocketInfo(const XMLElement*);

        void saveSystemInfo(const XMLElement*);
        void saveSocketInfo(const XMLElement*);

    private:
        static ConfigParser* 	m_pInstance;

        tagSystemInfo 			m_tagSysInfo;
        tagSocketInfo 			m_tagSocketInfo;

        XMLHelper* 				m_xmlHelper;
    };
}
#endif
