#ifndef __MULTIEPOLLHPP__
#define __MULTIEPOLLHPP__

namespace NET
{
	class CMultiEpoll : public CMultiManager::IMultiBase
	{
		public:
			CMultiEpoll();
			virtual ~CMultiEpoll();
			
			void	init() override;
			void	destroy() override;
			INT 	setSize(INT size) override;

			INT 	addFileEvent(INT, INT, EVENT_LOOP*) override;
			void	delFileEvent(INT, INT, EVENT_LOOP*) override;
			INT 	eventLoop(void*, EVENT_LOOP*, UINT) override; 			// struct timeval

			void	enableEdgeTrigger(BOOLEAN);

		private:
			INT                     m_epfd;
			BOOLEAN					m_bIsEdgeTrigger;
			struct epoll_event*     m_events;
	};
}
#endif
