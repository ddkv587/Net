#include "NET_CORE.hpp"

namespace NET 
{
#define		DEF_DEFAULT_VALUE	""
	XMLElement::XMLElement(xml_node<>* root)
		: m_uiElementSize(0)
		, m_uiAttributeSize(0)
		, strName(STRING_NULL)
		, strValue(STRING_NULL)
		, m_pXMLNode(root)
	{
		if ( NULL != root ) {
			xml_node<>* pNode = root->first_node();
			while ( pNode != root->last_node() )
				++m_uiElementSize;
		
			xml_attribute<>* pAtt = root->first_attribute();
			while ( pAtt != root->last_attribute() )
				++m_uiAttributeSize;

			strName 	= root->name();
			strValue 	= root->value(); 
		}
	}
			
	XMLElement::~XMLElement()
	{
		if ( NULL != m_pXMLNode ) {
			delete m_pXMLNode;
			m_pXMLNode = NULL;
		}
	}

	void XMLElement::setName(const STRING& strName)		
	{ 
		m_strName = strName; 

		m_pXMLNode->name(strName, strName.size());
	}

	void XMLElement::setValue(const STRING& strValue)		
	{
		m_strValue = strValue; 

		m_pXMLNode->value(strValue, strValue.size());
	}

	BOOLEAN	XMLElement::hasAttributes()
	{
		return m_uiAttributeSize != 0;
	}

	BOOLEAN XMLElement::hasAttribute(const STRING& strName)
	{
		return m_pXMLNode->first_attribute(strName.data(), strName.size()) != NULL;	
	}

	UINT XMLElement::getAttributeCount()
	{
		return m_uiAttributeSize;	
	}

	STRING	XMLElement::getAttributeByIndex(UINT uiIndex)
	{	
		if ( uiIndex >= m_uiAttributeSize ) return STRING_NULL;

		xml_attribute<>* att = m_pXMLNode->first_attribute();
       	for ( UINT ui=1; ui < uiIndex; ++ui ) {
	   		assert(NULL != att);
			att = att->next_attribute();
	   	}
		return att->value();
	}
			
	STRING XMLElement::getAttributeString(const STRING& strName) 
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att && 0 == strName.compare(att->name()) ) {
			return STRING( att->value() );
		}

		return STRING_NULL;
	}

	INT XMLElement::getAttributeINT(const STRING& strName)
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att && 0 == strName.compare(att->name()) ) {
			return atoi( att->value() );
		}

		return 0;
	}
		
	UINT XMLElement::getAttributeUINT(const STRING& strName)
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att && 0 == strName.compare(att->name()) ) {
			return UINT( atoi( att->value() ) );
		}

		return 0;
	}

	LLONG XMLElement::getAttributeLLONG(const STRING& strName)
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att && 0 == strName.compare(att->name()) ) {
			return atoll(att->value());
		}

		return 0;
	}

	FLOAT XMLElement::getAttributeFLOAT(const STRING& strName)
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att && 0 == strName.compare(att->name()) ) {
			return atof(att->value());
		}

		return 0.0f;
	}

	BOOLEAN XMLElement::getAttributeBOOLEAN(const STRING& strName)
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att && 0 == strName.compare(att->name()) ) {
			if ( strncmp(att->value(), "true", 4) == 0 || 
					strncmp(att->value(), "TRUE", 4) == 0 ) 
				return TRUE;
		}

		return FALSE;
	}

	void XMLElement::setAttribute(const STRING& strName, const STRING& strValue)
	{
		xml_attribute<>* att = getAttribute(strName, TRUE);
		if ( att && 0 == strName.compare(att->name()) ) {
			att->value(strValue.data(), strValue.size());
		}
	}

	void XMLElement::setAttribute(const STRING& strName, INT iValue)
	{
		xml_attribute<>* att = getAttribute(strName, TRUE);
		if ( att && 0 == strName.compare(att->name()) ) {
			STRING strValve = ::std::to_string(iValue);

			att->value(strValue.data(), strValue.size());
		}
	}

	void XMLElement::setAttribute(const STRING& strName, UINT uiValue)
	{
		xml_attribute<>* att = getAttribute(strName, TRUE);
		if ( att && 0 == strName.compare(att->name()) ) {
			STRING strValve = ::std::to_string(uiValue);

			att->value(strValue.data(), strValue.size());
		}
	}

	void XMLElement::setAttribute(const STRING& strName, LLONG lValue)
	{
		xml_attribute<>* att = getAttribute(strName, TRUE);
		if ( att && 0 == strName.compare(att->name()) ) {
			STRING strValve = ::std::to_string(lValue);

			att->value(strValue.data(), strValue.size());
		}	
	}

	void XMLElement::setAttribute(const STRING& strName, FLOAT fValue)
	{
		xml_attribute<>* att = getAttribute(strName, TRUE);
		if ( att && 0 == strName.compare(att->name()) ) {
			STRING strValve = ::std::to_string(fValue);

			att->value(strValue.data(), strValue.size());
		}
	}

	void XMLElement::setAttribute(const STRING& strName, BOOLEAN bValue)
	{
		xml_attribute<>* att = getAttribute(strName, TRUE);
		if ( att && 0 == strName.compare(att->name()) ) {
			STRING strValve = bValue ? "true" : "false";

			att->value(strValue.data(), strValue.size());
		}
	}

	void XMLElement::deleteAttribute(const STRING strName)
	{
		xml_attribute<>* att = getAttribute(strName);
		if ( att && 0 == strName.compare(att->name()) ) {
			m_pXMLNode->remove_attribute(att);

			--m_uiAttributeSize;
		}	
	}
	
	void XMLElement::clearAttributes()
	{
		assert( NULL != m_pXMLNode );
		m_pXMLNode->remove_all_attributes();

		m_uiAttributeSize = 0;
	}

	BOOLEAN XMLElement::hasElements()
	{
		return m_uiElementSize != 0;
	}

	BOOLEAN	XMLElement::hasElement(const STRING strName)
	{
		return m_pXMLNode->first_node(strName.data(), strName.) != NULL;	
	}

	UINT XMLElement::getElementsCount()
	{
		return m_uiElementSize;
	}

	XMLElement*	XMLElement::getElementByIndex(UINT uiIndex)
	{
		if ( uiIndex >= m_uiElementSize ) return NULL;

		xml_node<>* node = m_pXMLNode->first_node();
       	for ( UINT ui=1; ui < uiIndex; ++ui ) {
	   		assert(NULL != node);
			node = node->next_sibling();
	   	}
		
		if ( NULL != node ) return new XMLElement(node);

		return NULL;
	}

	XMLElement*	XMLElement::getElement(const STRING strName)
	{
		xml_node<>* node = m_pXMLNode->getElement(strName);
		if ( NULL != node ) return new XMLElement(node);

		return NULL;
	}
	
	XMLElement*	XMLElement::addElement(const STRING strName)
	{
		xml_node<>* node = getElement(strName, TRUE);
		if ( NULL != node ) return new XMLElement(node);

		return NULL;
	}
	
	void XMLElement::deleteElement(const STRING strName)
	{
		xml_node<>* node = getElement(strName);

		if ( NULL != node ) remove_node(node);
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
		xml_attribute<>* att = m_pXMLNode->first_attribute();
       	for ( UINT ui=0; ui < m_uiAttributeSize; ++ui ) {
			if ( att && 0 == strName.compare(att->name()) ) {
				return att;
			}
			att = att->next_attribute();
		}
		if ( bAdd ) {
			att = m_pXMLNode->allocate_attribute( 
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
		xml_node<>* node = m_pXMLNode->first_node();
       	for ( UINT ui=0; ui < m_uiElementSize; ++ui ) {
			if ( att && 0 == strName.compare(node->name()) ) {
				return node;
			}
			node = node->next_sibling();
		}	
		if ( bAdd ) {
			node = m_pXMLNode->allocate_node(
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
}
