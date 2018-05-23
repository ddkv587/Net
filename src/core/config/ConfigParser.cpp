#include "NET_Core.hpp"

namespace NET {
#define CONFIG_PATH                 "./config.xml"
#define CONFIG_PATH_BAK             "./.config.bak.xml"

#define TAG_SYSTEM_INFO             "SYSTEM"
#define TAG_SOCKET_INFO             "SOCKET"

    ConfigParser* ConfigParser::m_pInstance = NULL;

    ConfigParser* ConfigParser::getInstance() {
        if (NULL == m_pInstance) {
            m_pInstance = new ConfigParser();
        }
        return m_pInstance;
    }

    ConfigParser::ConfigParser()
    : m_xmlHelper(NULL) {
        preset();

        m_xmlHelper = new XMLHelper();
    }

    ConfigParser::~ConfigParser() {
        ;
    }

    BOOLEAN ConfigParser::initialize() {
        LOG(INFO) << "begin to initialize ConfigParser...";
        
        if (!load()) {
            LOG(WARNING) << "config file load error...";
            save();
        } else
            CMD::COPY(CONFIG_PATH, CONFIG_PATH_BAK);
        return TRUE;
    }

    void ConfigParser::preset() {
        m_tagSysInfo.uiPriority = 0; //nice value
        m_tagSysInfo.uiThreadCount = 10; //default max thread count
        m_tagSysInfo.uiShortTurnLimit = 100; 

        m_tagSocketInfo.uiPort = 8000;

        m_tagSocketInfo.bKeepAlive = TRUE;
        m_tagSocketInfo.uiAliveValue = 30;
        m_tagSocketInfo.uiTimeOut = 30;

        m_tagSocketInfo.bReusePort = FALSE;
        m_tagSocketInfo.bReuseAddress = TRUE;
    }

    BOOLEAN ConfigParser::load() {
        if (!m_xmlHelper->parseFrom(CONFIG_PATH))
            CHECK_R(m_xmlHelper->parseFrom(CONFIG_PATH_BAK), FALSE);

        XMLElement* root = m_xmlHelper->getRootElement();

        XMLElement* system = root->getElementByName(TAG_SYSTEM_INFO);
        if (NULL != system) loadSystemInfo(system);

        XMLElement* socket = root->getElementByName(TAG_SOCKET_INFO);
        if (NULL != socket) loadSocketInfo(socket);

        return TRUE;
    }

    BOOLEAN ConfigParser::save() {
        if (NULL == m_xmlHelper->getRootElement())
            m_xmlHelper->create();

        saveSystemInfo(m_xmlHelper->getRootElement());
        saveSocketInfo(m_xmlHelper->getRootElement());

        m_xmlHelper->saveTo(CONFIG_PATH);

        return TRUE;
    }

    void ConfigParser::loadSystemInfo(const XMLElement* system) {
        CHECK(NULL != system);
        
        XMLElement* priority = system->getElementByName("priority");
        if (NULL != priority) m_tagSysInfo.uiPriority = ::std::stoi(priority->getValue());
        
        XMLElement* threadCount = system->getElementByName("threadCount");
        if (NULL != threadCount) m_tagSysInfo.uiThreadCount = ::std::stoi(threadCount->getValue());
        
        XMLElement* shortTurnLimit = system->getElementByName("shortTurnLimit");
        if (NULL != shortTurnLimit) m_tagSysInfo.uiShortTurnLimit = ::std::stoi(shortTurnLimit->getValue());
        
        unsigned int hardware = ::std::thread::hardware_concurrency();
        m_tagSysInfo.uiThreadCount = MIN(hardware != 0 ? hardware : 6, m_tagSysInfo.uiThreadCount);
    }

    void ConfigParser::loadSocketInfo(const XMLElement* socket) {
        CHECK(NULL != socket);

        XMLElement* port = socket->getElementByName("port");
        if (NULL != port) m_tagSocketInfo.uiPort = ::std::stoi(port->getValue());

        XMLElement* alive = socket->getElementByName("keep-alive");
        if (NULL != alive) {
            m_tagSocketInfo.bKeepAlive = alive->getAttributeBOOLEAN("IsKeepAlive");
            m_tagSocketInfo.uiAliveValue = alive->getAttributeINT("KeepAliveValue");
            m_tagSocketInfo.uiTimeOut = alive->getAttributeINT("TimeOut");
        }

        XMLElement* reuse = socket->getElementByName("reuse");
        if (NULL != reuse) {
            m_tagSocketInfo.bReusePort = reuse->getAttributeBOOLEAN("IsReusePort");
            m_tagSocketInfo.bReuseAddress = reuse->getAttributeBOOLEAN("IsReuseAddress");
        }
    }

    void ConfigParser::saveSystemInfo(const XMLElement* root) {
        CHECK(NULL != root);

        XMLElement* system = root->addElement(TAG_SYSTEM_INFO);
        assert(system != NULL);
        
        XMLElement* priority = system->addElement("priority");
        assert(priority != NULL);
        priority->setValue(::std::to_string(m_tagSysInfo.uiPriority));
        
        XMLElement* threadCount = system->addElement("threadCount");
        assert(threadCount != NULL);
        threadCount->setValue(::std::to_string(m_tagSysInfo.uiThreadCount));
        
        XMLElement* shortTurnLimit = system->addElement("shortTurnLimit");
        assert(shortTurnLimit != NULL);
        shortTurnLimit->setValue(::std::to_string(m_tagSysInfo.uiShortTurnLimit));
    }

    void ConfigParser::saveSocketInfo(const XMLElement* root) {
        CHECK(NULL != root);

        XMLElement* socket = root->addElement(TAG_SOCKET_INFO);
        assert(socket != NULL);

        XMLElement* port = socket->addElement("port");
        assert(port != NULL);
        port->setValue(::std::to_string(m_tagSocketInfo.uiPort));

        XMLElement* alive = socket->addElement("keep-alive");
        assert(NULL != alive);
        alive->setAttribute("IsKeepAlive", m_tagSocketInfo.bKeepAlive);
        alive->setAttribute("KeepAliveValue", m_tagSocketInfo.uiAliveValue);
        alive->setAttribute("TimeOut", m_tagSocketInfo.uiTimeOut);

        XMLElement* reuse = socket->addElement("reuse");
        assert(NULL != reuse);
        reuse->setAttribute("IsReusePort", m_tagSocketInfo.bReusePort);
        reuse->setAttribute("IsReuseAddress", m_tagSocketInfo.bReuseAddress);
    }
}
