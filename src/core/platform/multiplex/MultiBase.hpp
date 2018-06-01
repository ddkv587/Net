#ifndef __MULTIPLEXMANAGERHPP__
#define __MULTIPLEXMANAGERHPP__

namespace NET
{
	class CMultiBase
	{
		public:
			enum EMultiType
			{
				EMT_NONE = -1,
				EMT_EPOLL,
				EMT_KQUEUE,
				EMT_SELECT,
			};

		public:
			virtual ~CMultiBase();

			EMultiType		getType() const 			{ return m_eType; }
			EVENT_LOOP* 	getEventLoop()				{ return m_eventLoop; }

			virtual int 	setSize(int size);

			virtual int 	addFileEvent(int fd, int mask);
			virtual void 	delFileEvent(int fd, int mask);
			virtual int		eventLoop(void* timeout);
			
			void			enableEdgeTrigger(BOOLEAN on = TRUE) 	{ m_bIsEdgeTrigger = on; }

			void			addTimer(const CTimer*);
			CTimer*			getNearestTimer();

		protected:
			CMultiBase(CMultiBase&) = delete;
			CMultiBase(const CMultiBase&) = delete;

		protected:
			CMultiBase();

			EMultiType				m_eType;
			EVENT_LOOP* 			m_eventLoop;
			
			BOOLEAN					m_bIsEdgeTrigger;
        
		private:
			::std::list<CTimer*>	m_lstTimer;
	};
}
#endif
