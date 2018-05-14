#include "NET_Core.hpp"

namespace NET 
{
#define CONFIG_PATH                 "./config.xml"
#define CONFIG_PATH_BAK             "./.config.bak.xml"
    
#define TAG_SYSTEM_INFO             "SYSTEM"
#define TAG_SOCKET_INFO             "SOCKET"

	ConfigParser* ConfigParser::m_pInstance = NULL;

	ConfigParser* ConfigParser::getInstance()
	{
		if ( NULL == m_pInstance ) {
			m_pInstance = new ConfigParser();
		}
		return m_pInstance;
	}

	ConfigParser::ConfigParser()
        : m_xmlHelper(NULL)
	{
        preset();
        
        m_xmlHelper = new XMLHelper();
	}

	ConfigParser::~ConfigParser()
	{
		;
	}

	BOOLEAN ConfigParser::initialize()
	{
        if( !load() ) {
            save();
        }
        return TRUE;
	}
    
    void ConfigParser::preset()
    {
        m_tagSysInfo.uiPriority         = 0;        //nice value
        m_tagSysInfo.uiThreadCount      = 10;       //default max thread count
        
        m_tagSocketInfo.uiPort          = 8000;
        
        m_tagSocketInfo.bKeepAlive      = TRUE;
        m_tagSocketInfo.uiAliveValue    = 30;
        m_tagSocketInfo.uiTimeOut       = 30;
        
        m_tagSocketInfo.bReusePort      = FALSE;
        m_tagSocketInfo.bReuseAddress   = TRUE;
    }
    
    BOOLEAN ConfigParser::load()
    {
        if ( !m_xmlHelper->parserFrom(CONFIG_PATH) )
            CHECK_R( m_xmlHelper->parserFrom(CONFIG_PATH_BAK), FALSE );
        
        XMLElement* root = m_xmlHelper->getRootElement();
        
        XMLElement* system = root->getElement(TAG_SYSTEM_INFO);
        if ( NULL != system ) buildSystemInfo(system);
        
        XMLElement* socket = root->getElement(TAG_SOCKET_INFO);
        if ( NULL != socket ) buildSocketInfo(socket)
        
        return TRUE;
    }
    
    BOOLEAN ConfigParser::save()
    {
        //if (  )
    }

	void ConfigParser::buildSystemInfo(const XMLElement* system)
	{
		m_tagSysInfo.m_uiThreadCount = 0;

		unsigned int hardware = ::std::thread::hardware_concurrency();
		m_tagSysInfo.m_uiThreadCount = MIN(hardware != 0 ? hardware : 6, m_tagSysInfo.m_uiThreadCount);
	}
    
    void ConfigParser::buildSocketInfo(const XMLElement* socket)
    {
        CHECK(NULL != socket);
        
        XMLElement* port = socket->getElement("port");
        if ( NULL != port ) m_tagSocketInfo.uiPort = port->getValue().toInt();
        
        XMLElement* alive = socket->getElement("keep-alive");
        if ( NULL != alive ) {
            m_tagSocketInfo.bKeepAlive      = alive->getAttributeBOOLEAN("IsKeepAlive");
            m_tagSocketInfo.uiAliveValue    = alive->getAttributeINT("KeepAliveValue");
            m_tagSocketInfo.uiTimeOut       = alive->getAttributeINT("TimeOut");
        }
        
        XMLElement* reuse = socket->getElement("reuse");
        if ( NULL != reuse ) {
            m_tagSocketInfo.bReusePort      = alive->getAttributeBOOLEAN("IsReusePort");
            m_tagSocketInfo.bReuseAddress   = alive->getAttributeBOOLEAN("IsReuseAddress");
        }
    }
}
