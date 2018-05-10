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
		if ( NULL != m_pXmlDoc ) delete m_pXmlDoc;
		if ( NULL != m_pRootElement ) delete m_pRootElement;
	}

	BOOLEAN	XMLHelper::parseFrom(const CHAR* strPath)
	{
		struct stat st;
		CHECK_R(-1 != stat(strPath, &st), false);
		
		INT fp = fopen(strPath, "r");
		CHAR* buff = new CHAR[st.st_size + 1];
		CHECK_R(buff != NULL, false);

		CHECK_R( st.st_size == read(fd, buff, st_size), false);
		buff[st.st_size] = 0;
		
		BOOLEAN ret = parse(buff);
		delete buff;

		return ret;
	}

	BOOLEAN XMLHelper::saveTo(const CHAR* strPath)
	{
		;
	}

	void XMLHelper::create()
	{
		m_pXmlDoc = new xml_document<>();

		STRING strHeader = "xml version=" + ( m_eVersion == VERSION_1_0 ? "'1.0'" : "'2.0'" ) + \
								" encoding=" + ( m_eEncoding == ENCODING_UTF8 ? "'utf-8'" : "'utf-16'");
		xml_node<>* pNode = m_xmlDoc->allocate_node( rapidxml::node_pi, \
					m_xmlDoc.allocate_string(strHeader) );

		m_pXmlDoc->append_node(pNode);
	}
	
	BOOLEAN XMLHelper::parse(const CHAR* strContent)
	{
		assert(m_pXmlDoc != NULL);
		try
		{
			m_xmlDoc.parse<0>(strContent);
	
		} catch (const rapidxml::parse_error& e) {
			LOG(WARNING) << e.what();
			
			return false;
		}

		return true;
	}

	void XMLHelper::deleteRootElement()
	{
		if ( NULL != m_pRootElement )
		{
			delete m_pRootElement;
			m_pRootElement = NULL;
		}
	}
}
