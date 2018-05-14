#include "NET_CORE.hpp"

namespace NET 
{
	XMLHelper(XMLVersion version, XMLEncoding encoding)
		: m_eVersion(version)
		, m_eEncoding(encoding)
		, m_pXmlDoc(NULL)
	 	, m_pRootElement(NULL)
	{
		;
	}

	XMLHelper::~XMLHelper()
	{
		if ( NULL != m_pRootElement ) delete m_pRootElement;
        
        if ( NULL != m_pXmlDoc ) delete m_pXmlDoc;
	}

	BOOLEAN	XMLHelper::parseFrom(const CHAR* strPath)
	{
		CHECK_R(-1 != stat(strPath, &st), FALSE);
       
        ifstream in(strPath);
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        
        STRING strBuff(size + 1, '\0');
        in.seekg(0, std::ios::beg);
        in.read(strBuff.data(), size);
        in.close();
   
        return parse(strBuff.data());
	}

	BOOLEAN XMLHelper::saveTo(const CHAR* strPath)
	{
        CHECK_R(m_pXmlDoc != NULL, FALSE);
        
        std::ofstream out(strPath);
        out << m_pXmlDoc;
        
        return TRUE;
	}

	void XMLHelper::create()
	{
		m_pXmlDoc = new xml_document<>();

		STRING strHeader = "xml version=" + \
                            ( m_eVersion == VERSION_1_0 ? "'1.0'" : "'2.0'" ) + \
                            " encoding=" + \
                            ( m_eEncoding == ENCODING_UTF8 ? "'utf-8'" : "'utf-16'");
        
		xml_node<>* node = m_xmlDoc->allocate_node( rapidxml::node_pi, \
					m_xmlDoc.allocate_string(strHeader) );
		m_pXmlDoc->append_node(node);
        
        m_pRootElement = new XMLElement(node);
        m_pRootElement.setName(L"ROOT");
	}
	
	BOOLEAN XMLHelper::parse(const CHAR* strContent)
	{
		assert(m_pXmlDoc != NULL);
		try
		{
			m_xmlDoc.parse<0>(strContent);
	
		} catch (const rapidxml::parse_error& e) {
			LOG(WARNING) << e.what();
			
			return FALSE;
		}
        
        xml_node<>* node = m_xmlDoc->first_node();
        if ( NULL != node ) m_pRootElement = new XMLElement(node);

		return TRUE;
	}

	void XMLHelper::deleteRootElement()
	{
		if ( NULL != m_pRootElement )
		{
			m_xmlDoc.remove_node(m_pRootElement->getXMLNode());
            m_pRootElement->setXMLNode(NULL);
            delete m_pRootElement;
			m_pRootElement = NULL;
		}
	}
}
