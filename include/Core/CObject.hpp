#ifndef __NET_COBJECT_HPP__
#define __NET_COBJECT_HPP__

#include "TypeDefine.hpp"

namespace NET
{
	class CObject
	{
	public:// TODO: define const here

	private:// TODO: define const here

	public:// TODO: define embed class or struct or enum here

	private:// TODO: define embed class or struct or enum here

	public:// TODO: define your public method here
        const KEY id()          { return m_key == -1 ? reinterpret_cast<KEY>(this) : m_key; }

	protected:// TODO: define your protected method here
		CObject() : m_key(-1)   { }
        virtual ~CObject()      { }

	private:// TODO: define your private method here
		
	protected:// property
        KEY                m_key;

	private:// property

	};
}

#endif  // __NET_COBJECT_HPP__