#ifndef __MULTIEPOLLHPP__
#define __MULTIEPOLLHPP__

namespace NET
{
	class CMultiEpoll : public CMultiBase
	{
		public:
			CMultiEpoll();
			virtual ~CMultiEpoll();

			int 	addFileEvent(int, int);
			void	delFileEvent(int, int);
			int 	eventLoop(void*); // struct timeval

			private:
			int m_epfd;
			struct epoll_event *m_events;
	};
}
#endif
