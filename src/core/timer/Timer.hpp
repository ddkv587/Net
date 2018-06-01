#ifndef __TIMERHPP__
#define __TIMERHPP__

namespace NET 
{
	class CTimer final 
	{
		public:
			explicit CTimer() noexcept;
			CTimer(UINT, INT) noexcept;
			~CTimer() noexcept;

			inline void 	setInterval(UINT interval) noexcept { m_uiInterval = interval; }
			inline UINT		interval() noexcept { return m_uiInterval; }

			inline void 	setRepeat(INT repeat) noexcept { m_iRepeat = repeat; }
			inline INT		repeat() noexcept { return m_iRepeat; }

			void 			start() noexcept;
			void 			stop() noexcept;
			void 			reset() noexcept;
			BOOLEAN			isRunning() noexcept { return m_bIsRunning; }

			void 			addListener(const ITimerListener*) noexcept;
			void			delListener(const ITimerListener*) noexcept;

			CTimer& 		operator=(CTimer&&);	//std::move

		protected:
			CTimer(CTimer&) = delete;
			CTimer(const CTimer&) = delete;
			CTimer& operator=(const CTimer&) = delete;	

		private:
			UINT		m_uiInterval;
			INT			m_iRepeat;		// -1 means repeat loop
			BOOLEAN		m_bIsRunning;

			::std::mutex					m_mutex;			
			::std::vector<ITimerListener*> 	m_lstListener;
	};
}
#endif
