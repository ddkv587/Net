#ifndef __CMESSAGEQUEUEHPP__
#define __CMESSAGEQUEUEHPP__

namespace NET
{
	class CMessageQueue
	{
		public:
			CMessageQueue(){}
			virtual ~CBaseMessage {}

		private:
			::std::vector<CBaseMessage> m_msgQueue;  
	};
}
#endif
