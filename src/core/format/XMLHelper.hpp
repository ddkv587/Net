#ifndef __XMLHELPERHPP__
#define __XMLHELPERHPP__

namespace NET {

    class XMLHelper {
    public:

        enum XMLVersion {
            VERSION_1_0,
            VERSION_2_0
        };

        enum XMLEncoding {
            ENCODING_UTF8,
            ENCODING_UTF16,
        };

    public:
        XMLHelper(XMLVersion version = VERSION_1_0, XMLEncoding encoding = ENCODING_UTF8);
        virtual ~XMLHelper();

        BOOLEAN parseFrom(const CHAR* strPath);
        BOOLEAN saveTo(const CHAR* strPath);

        void create();
        BOOLEAN parse(CHAR* strContent);

        BOOLEAN hasRootElement() {
            return m_pRootElement == NULL;
        }

        XMLElement* getRootElement() {
            return m_pRootElement;
        }

        void deleteRootElement();

    private:
        XMLVersion m_eVersion;
        XMLEncoding m_eEncoding;

        xml_document<>* m_pXmlDoc;
        XMLElement* m_pRootElement;
    };
}
#endif
