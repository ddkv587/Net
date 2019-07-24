#ifndef __CTHREADHPP__
#define __CTHREADHPP__

namespace NET
{
	class CThread
	{
	public:
		enum EPolicy
		{
			SCHED_OTHER = 0,
			SCHED_BATCH,
			SCHED_IDLE,
			SCHED_FIFO,
			SCHED_RR
		};

	public:
		CThread()
			: m_bStop( FALSE )
			, m_iPriority(0)
		{
			;
		}

		virtual ~CThread() 
		{
			if ( m_thread.joinable() ) {
				m_thread.join();
			}
		}

		void								start( void* arg = nullptr ) 					{ m_thread	= ::std::thread(&CThread::mainLoop, this, std::move(arg)); }
		void 								stop()											{ m_bStop	= TRUE; }
		cosnt ::std::thread::id 			threadID()										{ return m_thread.get_id(); }

		inline cosnt ::std::string 			name()											{ return m_strName; }
		void 								setName( const ::std::string& strName )			{ m_strName = strName; }

		// ============= priority ===================
		inline EPolicy 						policy()										{ return m_ePolicy; }			
		inline INT 							priority()										{ return m_iPriority; }
		BOOLEAN 							setPriority( INT iPriority, EPolicy policy );

		// ============= affinity ===================
		//const UINT*							affinity();
		BOOLEAN								setAffinity( const UINT[]& cpus );

		CThread(CThread&) = delete;
		CThread(const CThread&) = delete;
		CThread& operator=(const CThread&) = delete;

	protected:
		virtual void mainLoop(void* arg)
		{
			UNUSED(arg);
			while ( !m_bStop ) {
				::std::this_thread::sleep_for( ::std::chrono::milliseconds( 1000 ) );
			}
		}

	private:
		INT									transformPolicy( EPolicy policy );

	protected:
		BOOLEAN 				m_bStop;
		EPolicy					m_ePolicy;
		UINT 					m_iPriority;

	private:
		::std::string			m_strName;
		::std::thread 			m_thread;
	};
}
#endif
