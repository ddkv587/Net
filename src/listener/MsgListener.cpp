#include "NET_Listener.hpp"

namespace NET 
{
	CMsgListener::CMsgListener(int id, struct sockaddr_in addr)
		: m_clientID(id)
		, m_clientAddr(addr) 
	{
		m_type = EMT_LISTENER;
	}

	CMsgListener::~CMsgListener()
	{
		;
	}

}
