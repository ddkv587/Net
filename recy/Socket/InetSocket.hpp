#ifndef __INETSOCKETH__
#define __INETSOCKETH__

#include "CSocket.hpp"

namespace Net
{
	using std::string;

	class CInetSocket : public CSocket
	{
		protect:
			string 	m_strHost;
			string 	m_strPort;
			int 	m_iProtocol;	

		public:
			CInetSocket() : m_strHost(""), m_strPort(""), m_iProtocol(-1)
			{}
		
			const string& getHost() { return m_strHost; }
			const string& getPort()	{ return m_strPort; }
	};
}
#endif
