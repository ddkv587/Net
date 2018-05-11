#ifndef __XMLELEMENTHPP__
#define __XMLELEMENTHPP__

namespace NET 
{
	class XMLElement
	{
		public:
			XMLElement(xml_node<>* root = NULL);
			virtual ~XMLElement();

			//name 
			const STRING&			getName()							{ return m_strName }
			void					setName(const STRING& strName);

			//text
			const STRING&			getValue()							{ return m_strValue; }
			void					setValue(const STRING& strValue);

			// attributes
			BOOLEAN					hasAttributes();
			BOOLEAN 				hasAttribute(const STRING& strName);
			UINT 					getAttributeCount();
			STRING					getAttributeByIndex(UINT uiIndex);
			
			STRING					getAttributeString(const STRING& strName) const;
			INT						getAttributeINT(const STRING& strName);
			UINT					getAttributeUINT(const STRING& strName);
			LLONG					getAttributeLLONG(const STRING& strName);
			FLOAT					getAttributeFLOAT(const STRING& strName);
			BOOLEAN					getAttributeBOOLEAN(const STRING& strName);

			void					setAttribute(const STRING& strName, const STRING& strValue);
			void					setAttribute(const STRING& strName, INT iValue);
			void					setAttribute(const STRING& strName, UINT uiValue);
			void					setAttribute(const STRING& strName, LLONG lValue);
			void					setAttribute(const STRING& strName, FLOAT fValue);
			void					setAttribute(const STRING& strName, BOOLEAN bValue);

			void					deleteAttribute(const STRING strName);
			void					clearAttributes();

			//child
			BOOLEAN					hasElements();
			BOOLEAN					hasElement(const STRING strName);
			UINT					getElementsCount();
			XMLElement*				getElementByIndex(UINT uiIndex);
			XMLElement*				getElement(const STRING strName);
	
			XMLElement*				addElement(const STRING strName);
			BOOLEAN					deleteElement(const STRING strName);
			BOOLEAN					clearElements();

		protected:
			void					setXMLNode(xml_node<>* pNode)		{ m_pXMLNode = pNode; }
			xml_node<>*				getXMLNode() const 					{ return m_pXMLNode; }

		private:
			BOOLEAN					checkString(const STRING& str);
			xml_attribute<>*		getAttribute(const STRING& strName, BOOLEAN bAdd = FALSE);
			xml_node<>*				getElement(const STRING& strName, BOOLEAN bAdd = FALSE);
	
		private:
			UINT					m_uiElementSize;
			UINT					m_uiAttributeSize;

			STRING					m_strName;
			STRING					m_strValue;
			xml_node<>*				m_pXMLNode;
	};
}
#endif
