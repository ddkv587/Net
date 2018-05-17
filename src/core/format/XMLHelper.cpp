#include "NET_Core.hpp"

namespace NET 
{
	XMLHelper::XMLHelper(XMLVersion version, XMLEncoding encoding)
		: m_eVersion(version)
		, m_eEncoding(encoding)
		, m_pXmlDoc(NULL)
	 	, m_pRootElement(NULL)
	{
		m_pXmlDoc = new xml_document<>();
		STRING strHeader = STRING("xml version=") + \
                            ( m_eVersion == VERSION_1_0 ? STRING("'1.0'") : STRING("'2.0'") ) + \
                           	STRING(" encoding=") + \
                            ( m_eEncoding == ENCODING_UTF8 ? STRING("'utf-8'") : STRING("'utf-16'"));
        
		xml_node<>* header = m_pXmlDoc->allocate_node( node_pi, \
					m_pXmlDoc->allocate_string(strHeader.data(), strHeader.size()) );
		m_pXmlDoc->append_node(header);

		create();
	}

	XMLHelper::~XMLHelper()
	{
		if ( NULL != m_pRootElement ) delete m_pRootElement;
        
        if ( NULL != m_pXmlDoc ) delete m_pXmlDoc;
	}

	BOOLEAN	XMLHelper::parseFrom(const CHAR* strPath)
	{
        ::std::ifstream in(strPath);
        in.seekg(0, in.end);
        size_t size = in.tellg();
        
        CHAR* strBuff = new CHAR[size + 1];
		strBuff[size] = '\0';
        in.seekg(0, in.beg);
        in.read(strBuff, size);
        in.close();
   
        return parse(strBuff);
	}

	BOOLEAN XMLHelper::saveTo(const CHAR* strPath)
	{
        CHECK_R(m_pXmlDoc != NULL, FALSE);
        
        ::std::ofstream out(strPath);
        out << (*m_pXmlDoc);
        
        return TRUE;
	}

	void XMLHelper::create()
	{
		xml_node<>* node = m_pXmlDoc->allocate_node( node_element, "ROOT");
        m_pXmlDoc->append_node(node);

        m_pRootElement = new XMLElement(node);
        m_pRootElement->setName("ROOT");
	}
	
	BOOLEAN XMLHelper::parse(CHAR* strContent)
	{
		try {
			m_pXmlDoc->parse<0>(strContent);
		} catch (const rapidxml::parse_error& e) {
			LOG(WARNING) << e.what();

			delete m_pRootElement; 
			m_pRootElement = NULL;	
			return FALSE;
		}
        
        xml_node<>* node = m_pXmlDoc->first_node();
        if ( NULL != node ) m_pRootElement = new XMLElement(node);

		return TRUE;
	}

	void XMLHelper::deleteRootElement()
	{
		if ( NULL != m_pRootElement ) {
			m_pXmlDoc->remove_node(m_pRootElement->getXMLNode());
            m_pRootElement->setXMLNode(NULL);
            delete m_pRootElement;
			m_pRootElement = NULL;
		}
	}
}
