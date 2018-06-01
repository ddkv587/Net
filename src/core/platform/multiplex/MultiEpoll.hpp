#ifndef __MULTIEPOLLHPP__
#define __MULTIEPOLLHPP__

namespace NET
{
	class CMultiEpoll : public CMultiBase
	{
		public:
			CMultiEpoll();
			virtual ~CMultiEpoll();

			int 	setSize(int size);

			int 	addFileEvent(int, int) override;
			void	delFileEvent(int, int) override;
			int 	eventLoop(void* timeout = NULL) override; // struct timeval

		private:
			int                     m_epfd;
			struct epoll_event*     m_events;
	};
}
#endif
