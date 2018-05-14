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

			int 	addFileEvent(int, int);
			void	delFileEvent(int, int);
			int 	eventLoop(void* timeout = NULL); // struct timeval

		private:
			int                     m_epfd;
			struct epoll_event*     m_events;
	};
}
#endif
