#ifndef __STREAM_HPP__
#define __STREAM_HPP__

#include "Socket.hpp"
namespace Net
{
	class CStream : public CSocket
	{
		protect:
			bool	m_bReadable;
			bool  	m_bWritable;

		public:
			
	};
}

#endif
