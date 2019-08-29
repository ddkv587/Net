#ifndef __NET_COBJECT_HPP__
#define __NET_COBJECT_HPP__

#include "TypeDefine.hpp"

namespace NET
{
    class CObject
    {
    public:
        const KEY id()          { return m_key == -1 ? reinterpret_cast<KEY>(this) : m_key; }

    protected:
        CObject() : m_key(-1)   { }
        virtual ~CObject()      { }

    private:
        CObject(CObject&) = delete;
        CObject(const CObject&) = delete;
        CObject& operator=(const CObject&) = delete;

    protected:
        KEY                m_key;
    };
}

#endif