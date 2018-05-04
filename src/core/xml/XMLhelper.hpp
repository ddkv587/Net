#ifndef __XMLHELPERHPP__
#define __XMLHELPERHPP__

namespace NET
{
	class XMLHelper
	{
		public:
			enum XMLVersion
			{
				VERSION_1_0,
				VERSION_2_0
			};

			enum XMLEncoding
			{
				ENCODING_UTF8,
				ENCODING_UTF16,
			};

		public:
			XMLHelper(XMLVersion version = VERSION_1_0, XMLEncoding encoding = ENCODING_UTF8);
			virtual ~XMLHelper();

			BOOLEAN 				parseFrom(const CHAR* strPath);
			BOOLEAN					saveTo(const CHAR* strPath);

			void					initialize();
			BOOLEAN					parse(const CHAR* strContent);

			//name 
			::std::string			getName() const;
			void					setName(const ::std::string strName);

			//text
			::std::string			getText() const;
			void					setText(const ::std::string strText);

			// attributes
			BOOLEAN					hasAttributes();
			BOOLEAN 				hasAttribute(const ::std::string& strName);
			UINT 					getAttributeCount();
			::std::string			getAttributeByIndex(UINT uiIndex);
			
			::std::string&			getAttributeString(const ::std::string& strName) const;
			INT						getAttributeINT(const ::std::string& strName);
			UINT					getAttributeUINT(const ::std::string& strName);
			LLONG					getAttributeLLONG(const ::std::string& strName);
			FLOAT					getAttributeFLOAT(const ::std::string& strName);
			BOOLEAN					getAttributeBOOLEAN(const ::std::string& strName);

			void					setAttribute(const ::std::string& strName, const ::std::string& strValue);
			void					setAttribute(const ::std::string& strName, INT iValue);
			void					setAttribute(const ::std::string& strName, UINT uiValue);
			void					setAttribute(const ::std::string& strName, LLONG lValue);
			void					setAttribute(const ::std::string& strName, FLOAT fValue);
			void					setAttribute(const ::std::string& strName, BOOLEAN bValue);

			void					deleteAttribute(const ::std::string strName);
			void					clearAttributes();

			//child
			BOOLEAN					hasElements();
			BOOLEAN					hasElement(const ::std::string strName);
			UINT					getElementsCount();
			POINT					getElementByIndex(UINT uiIndex);
			POINT					getElement(const ::std::string strName);
	
			POINT					addElement(const ::std::string strName);
			BOOLEAN					deleteElement(const ::std::string strName);
			BOOLEAN					clearElements();

		private:
			XMLVersion				m_eVersion;
			XMLEncoding				m_eEncoding;

			xml_document<>			m_xmlDoc;
	};
}
#endif
