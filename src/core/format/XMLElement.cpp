#include "NET_Core.hpp"

namespace NET 
{
#define		DEF_DEFAULT_VALUE	""
	XMLElement::XMLElement(xml_node<>* root)
        : m_strName(STRING_NULL)
		, m_strValue(STRING_NULL)
		, m_pXMLNode(root)
	{
        assert ( NULL != root );
        
        m_strName 	= root->name();
        m_strValue 	= root->value();
	}
			
	XMLElement::~XMLElement()
	{
        ;
	}

	void XMLElement::setName(const STRING& strName)		
	{ 
		m_strName = strName; 

		m_pXMLNode->name(strName.data(), strName.size());
	}

	void XMLElement::setValue(const STRING& strValue)		
	{
		m_strValue = strValue; 

		m_pXMLNode->value(strValue.data(), strValue.size());
	}

	BOOLEAN	XMLElement::hasAttributes()
	{
		return  m_pXMLNode->first_attribute() != NULL;
	}

	BOOLEAN XMLElement::hasAttribute(const STRING& strName)
	{
		return m_pXMLNode->first_attribute(strName.data(), strName.size()) != NULL;	
	}

	UINT XMLElement::getAttributeCount()
	{
        UINT uiCount = 0;
        xml_attribute<>* att = m_pXMLNode->first_attribute();
        while ( att && att != m_pXMLNode->last_attribute() ) {
            ++uiCount;
            att = att->next_attribute();
        }
        
        return uiCount;
	}

	STRING XMLElement::getAttributeByIndex(UINT uiIndex)
	{
		xml_attribute<>* att = m_pXMLNode->first_attribute();
       	for ( UINT ui=1; ui < uiIndex; ++ui ) {
            if ( NULL == att ) return STRING_NULL;
			att = att->next_attribute();
	   	}
		return att->value();
	}
			
	STRING XMLElement::getAttributeString(const STRING& strName) 
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att ) return STRING( att->value() );

		return STRING_NULL;
	}

	INT XMLElement::getAttributeINT(const STRING& strName)
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att ) return atoi( att->value() );

		return 0;
	}
		
	UINT XMLElement::getAttributeUINT(const STRING& strName)
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att ) return UINT( atoi( att->value() ) );

		return 0;
	}

	LLONG XMLElement::getAttributeLLONG(const STRING& strName)
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att ) return atoll(att->value());

		return 0;
	}

	FLOAT XMLElement::getAttributeFLOAT(const STRING& strName)
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att ) return atof(att->value());

		return 0.0f;
	}

	BOOLEAN XMLElement::getAttributeBOOLEAN(const STRING& strName)
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att ) {
			if ( strncmp(att->value(), "true", 4) == 0 || 
					strncmp(att->value(), "TRUE", 4) == 0 ) 
				return TRUE;
		}

		return FALSE;
	}

	void XMLElement::setAttribute(const STRING& strName, const STRING& strValue)
	{
		xml_attribute<>* att = getAttribute(strName, TRUE);
		if ( att ) att->value(strValue.data(), strValue.size());
	}

	void XMLElement::setAttribute(const STRING& strName, INT iValue)
	{
		xml_attribute<>* att = getAttribute(strName, TRUE);
		if ( att ) {
			STRING strValue = ::std::to_string(iValue);

			att->value(strValue.data(), strValue.size());
		}
	}

	void XMLElement::setAttribute(const STRING& strName, UINT uiValue)
	{
		xml_attribute<>* att = getAttribute(strName, TRUE);
		if ( att ) {
			STRING strValue = ::std::to_string(uiValue);

			att->value(strValue.data(), strValue.size());
		}
	}

	void XMLElement::setAttribute(const STRING& strName, LLONG lValue)
	{
		xml_attribute<>* att = getAttribute(strName, TRUE);
		if ( att ) {
			STRING strValue = ::std::to_string(lValue);

			att->value(strValue.data(), strValue.size());
		}	
	}

	void XMLElement::setAttribute(const STRING& strName, FLOAT fValue)
	{
		xml_attribute<>* att = getAttribute(strName, TRUE);
		if ( att ) {
			STRING strValue = ::std::to_string(fValue);

			att->value(strValue.data(), strValue.size());
		}
	}

	void XMLElement::setAttribute(const STRING& strName, BOOLEAN bValue)
	{
		xml_attribute<>* att = getAttribute(strName, TRUE);
		if ( att ) {
			STRING strValue = bValue ? "true" : "false";

			att->value(strValue.data(), strValue.size());
		}
	}

	void XMLElement::deleteAttribute(const STRING& strName)
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att ) m_pXMLNode->remove_attribute(att);
	}
	
	void XMLElement::clearAttributes()
	{
		assert( NULL != m_pXMLNode );
		m_pXMLNode->remove_all_attributes();
	}

	BOOLEAN XMLElement::hasElements()
	{
		return m_pXMLNode->first_node() != NULL;
	}

	BOOLEAN	XMLElement::hasElement(const STRING& strName)
	{
		return m_pXMLNode->first_node(strName.data(), strName.size()) != NULL;	
	}

	UINT XMLElement::getElementsCount()
	{
        UINT uiCount = 0;
        
        xml_node<>* node = m_pXMLNode->first_node();
        while ( node && node != m_pXMLNode->last_node() )
        {
            ++uiCount;
            node = node->next_sibling();
        }
        
        return uiCount;
	}

	XMLElement*	XMLElement::getElementByIndex(UINT uiIndex)
    {
		xml_node<>* node = m_pXMLNode->first_node();
       	for ( UINT ui=1; ui < uiIndex; ++ui ) {
            if ( NULL == node ) return NULL;
			node = node->next_sibling();
	   	}
		
		if ( NULL != node ) return new XMLElement(node);

		return NULL;
	}

	XMLElement*	XMLElement::getElementByName(const STRING& strName) const
	{
		xml_node<>* node = m_pXMLNode->first_node(strName.data(), strName.size());
		if ( NULL != node ) return new XMLElement(node);

		return NULL;
	}
	
	XMLElement*	XMLElement::addElement(const STRING& strName)
	{
		xml_node<>* node = getElement(strName, TRUE);
		if ( NULL != node ) return new XMLElement(node);

		return NULL;
	}
	
	void XMLElement::deleteElement(const STRING& strName)
	{
		xml_node<>* node = getElement(strName);

		if ( NULL != node ) m_pXMLNode->remove_node(node);
	}

	void XMLElement::clearElements()
	{
		m_pXMLNode->remove_all_nodes();
	}
	
	BOOLEAN XMLElement::checkString(const STRING& str)
	{
		return str.back() == '\0';
	}

	xml_attribute<>* XMLElement::getAttribute(const STRING& strName, BOOLEAN bAdd)
	{
		xml_attribute<>* att = m_pXMLNode->first_attribute(strName.data(), strName.size());
		if ( NULL != att ) return att;
		
		if ( bAdd ) {
			CHECK_R( getXMLDocument(), NULL );
			att = m_pXMLDocument->allocate_attribute( 
					strName.data(), \
					NULL, \
					strName.size(), \
					0);
			m_pXMLNode->append_attribute(att);
			return att;
		}
		return NULL;
	}

	xml_node<>*	XMLElement::getElement(const STRING& strName, BOOLEAN bAdd)
	{
		xml_node<>* node = m_pXMLNode->first_node(strName.data(), strName.size());
		if ( NULL != node ) return node;

		if ( bAdd ) {
			CHECK_R( getXMLDocument(), NULL );
			node = m_pXMLDocument->allocate_node(
					rapidxml::node_element, \
					strName.data(), \
					NULL, \
					strName.size(), \
					0);
			m_pXMLNode->append_node(node);
			return node;
		}
		return NULL;
	}

	xml_document<>*	XMLElement::getXMLDocument()
	{
		if ( m_pXMLDocument ) return m_pXMLDocument;

		if ( m_pXMLNode ) {
			m_pXMLDocument = m_pXMLNode->document();
			return m_pXMLDocument;
		}

		return NULL;
	}
}
