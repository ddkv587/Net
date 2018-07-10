#include "NET_Core.hpp"

namespace  NET
{
	CMultiSelect::CMultiSelect()
    {
        ;
    }

	CMultiSelect::~CMultiSelect()
    {
        ;
    }

	void CMultiSelect::init()
    {
        ;
    }
		
    void CMultiSelect::destroy()
    {
        ;
    }

	UINT CMultiSelect::setSize(UINT uiSize)
    {
        UNUSED(uiSize);
        return 0;
    }

	INT CMultiSelect::addFileEvent(INT, INT, EVENT_LOOP*)
    {
        return 0;
    }

	void CMultiSelect::delFileEvent(INT, INT, EVENT_LOOP*)
    {
        ;
    }
	
    INT CMultiSelect::eventLoop(void*, ::std::vector<FIRED_EVENT>&)
    {
        return 0;
    }
}