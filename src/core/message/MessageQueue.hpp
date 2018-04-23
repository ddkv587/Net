#ifndef __CMESSAGEQUEUEHPP__
#define __CMESSAGEQUEUEHPP__

namespace NET
{
	class CMessageQueue
	{
		public:
			CMessageQueue();
			virtual ~CMessageQueue();

			virtual void 			pushMesssage(CBaseMessage*);
			virtual CBaseMessage* 	popMessage();

			bool					isEmpty();

		protected:
			CMessageQueue(CMessageQueue&) = delete;
			CMessageQueue(const CMessageQueue&) = delete;
			CMessageQueue& operate=(const CMessageQueue&) = delete;

		private:
			::std::mutex					m_mutex;
			::std::queue<CBaseMessage*> 	m_msgQueue;  
	};
}
#endif
