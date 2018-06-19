#ifndef __MULTIMANAGERHPP__
#define __MULTIMANAGERHPP__

namespace NET
{
	class CMultiManager
	{
		public:
			class IMultiBase
			{
				public:
					virtual ~IMultiBase() {}

					virtual	void			init() = 0;
					virtual void			destroy() = 0;

					virtual INT             setSize(INT size) = 0;
					virtual INT             addFileEvent(INT fd, INT mask, EVENT_LOOP* eventLoop) = 0;
					virtual void            delFileEvent(INT fd, INT mask, EVENT_LOOP* eventLoop) = 0;
					virtual	INT				eventLoop(void* timeout, EVENT_LOOP* eventLoop) = 0;
			};

			enum EMultiType
			{
				EMT_NONE = -1,
				EMT_EPOLL,
				EMT_KQUEUE,
				EMT_SELECT,
			};
		
		private:

		public:
			CMultiManager();
			virtual ~CMultiManager();

			void					init(EMultiType type = EMT_EPOLL);
			void 					destroy();
			INT						setSize(INT size);

			inline EMultiType		getMultiType()					{ return m_eType; }
			const EVENT_LOOP*		getEventLoop()					{ return m_pEventLoop; }

			INT						addFileEvent(INT, INT);
			void					delFileEvent(INT, INT);
			INT						eventLoop(void*);
			void					addTimer(const CTimer*);

			//for epoll
			void					enableEdgeTrigger(BOOLEAN on = TRUE);

		private:
			const CTimer*			getNearestTimer();

		private:
			EMultiType				m_eType;
			
			IMultiBase* 			m_pBase;
			EVENT_LOOP*				m_pEventLoop;
	};
}
#endif
