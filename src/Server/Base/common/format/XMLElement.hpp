#ifndef __XMLELEMENTHPP__
#define __XMLELEMENTHPP__

namespace NET {

    class XMLElement {
    public:
        XMLElement(xml_node<>* root);
        virtual ~XMLElement();

        // name 
        STRING  getName() { return m_strName; }
        void    setName(const STRING& strName);

        // text
        STRING  getValue() { return m_strValue; }
        void    setValue(const STRING& strValue);

        // attributes
        BOOLEAN hasAttributes();
        BOOLEAN hasAttribute(const STRING& strName);
        UINT    getAttributeCount();
        STRING  getAttributeByIndex(UINT uiIndex);

        STRING  getAttributeString(const STRING& strName);
        INT     getAttributeINT(const STRING& strName);
        UINT    getAttributeUINT(const STRING& strName);
        LLONG   getAttributeLLONG(const STRING& strName);
        FLOAT   getAttributeFLOAT(const STRING& strName);
        BOOLEAN getAttributeBOOLEAN(const STRING& strName);

        void setAttribute(const STRING& strName, const STRING& strValue);
        void setAttribute(const STRING& strName, INT iValue);
        void setAttribute(const STRING& strName, UINT uiValue);
        void setAttribute(const STRING& strName, LLONG lValue);
        void setAttribute(const STRING& strName, FLOAT fValue);
        void setAttribute(const STRING& strName, BOOLEAN bValue);

        void deleteAttribute(const STRING& strName);
        void clearAttributes();

        // child
        BOOLEAN     hasElements();
        BOOLEAN     hasElement(const STRING& strName);
        UINT        getElementsCount();
        XMLElement* getElementByIndex(UINT uiIndex);
        XMLElement* getElementByName(const STRING& strName) const;

        XMLElement* addElement(const STRING& strName) const;
        void        deleteElement(const STRING& strName);
        void        clearElements();

    protected:
        void setXMLNode(xml_node<>* pNode)  { m_pXMLNode = pNode; }
        xml_node<>* getXMLNode() const      { return m_pXMLNode; }

        friend class XMLHelper;

    private:
        BOOLEAN             checkString(const STRING& str);
        xml_attribute<>*    getAttribute(const STRING& strName, BOOLEAN bAdd = FALSE) const noexcept;
        xml_node<>*         getElement(const STRING& strName, BOOLEAN bAdd = FALSE) const noexcept;

    private:
        STRING          m_strName;
        STRING          m_strValue;
        xml_node<>      *m_pXMLNode;
        xml_document<>  *m_pXMLDocument;
    };
}
#endif
