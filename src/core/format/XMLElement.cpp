#include "NET_CORE.hpp"

namespace NET 
{
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
		return m_xmlDoc->first_attribute() != NULL;
	}

	BOOLEAN XMLElement::hasAttribute(const STRING& strName)
	{
		return m_xmlDoc->first_attribute(strName, strName.size()) != NULL;	
	}

	UINT XMLElement::getAttributeCount()
	{
		for (xml_attribute<Ch> *attribute = m_first_attribute; attribute; attribute = attribute->m_next_attribute)
	
	}

			STRING&					getAttributeByIndex(UINT uiIndex);
			
			STRINg&					getAttributeString(const STRING& strName) const;
			INT						getAttributeINT(const STRING& strName);
			UINT					getAttributeUINT(const STRING& strName);
			LLONG					getAttributeLLONG(const STRING& strName);
			FLOAT					getAttributeFLOAT(const STRING& strName);
			BOOLEAN					getAttributeBOOLEAN(const STRING& strName);


	XMLElement*	XMLElement::addElement(const STRING strName)
	{
		assert(NULL != m_pXMLNode);
	
		XMLElement* pElement = new XMLElement;
		pElement->setName(strName);
		pElement->setXMLNode(m_pXMLNode.allocate_node(rapidxml::node_element, strName, ""));
	}
	
	BOOLEAN	XMLElement::deleteElement(const STRING strName)
	{
		;
	}

	BOOLEAN	XMLElement::clearElements()
	{
		;
	}
	
	BOOLEAN XMLElement::checkString(const STRING& str)
	{
		return str.back() == '\0';
	}
}
