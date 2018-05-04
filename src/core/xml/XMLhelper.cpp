#include "NET_CORE.hpp"

namespace NET 
{
	XMLHelper::XMLHelper()
	{
		;
	}

	virtual XMLHelper::~XMLHelper()
	{
		;
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

	void XMLHelper::initialize()
	{
		m_xmlDoc.allocate_node(rapidxml::node_pi, 
					m_xmlDoc.allocate_string("xml version='1.0' encoding='utf-8'"));
	}
	
	BOOLEAN XMLHelper::parse(const CHAR* strContent)
	{
		try
		{
			m_xmlDoc.parse<0>(strContent);
	
		} catch (const rapidxml::parse_error& e) {
			LOG(WARNING) << e.what();
			
			return false;
		}

		return true;
	}
}
