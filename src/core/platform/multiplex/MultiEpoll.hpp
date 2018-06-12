#ifndef __MULTIEPOLLHPP__
#define __MULTIEPOLLHPP__

namespace NET
{
	class CMultiEpoll : public CMultiBase
	{
		public:
			CMultiEpoll();
			virtual ~CMultiEpoll();
			
			void	init() override;
			void	destroy() override;
			INT 	setSize(INT size) override;

			INT 	addFileEvent(INT, INT) override;
			void	delFileEvent(INT, INT) override;
			INT 	eventLoop(void*) override; 			// struct timeval

		private:
			INT                     m_epfd;
			struct epoll_event*     m_events;
	};
}
#endif
