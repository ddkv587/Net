#ifndef __NET_CTHREADHPP__
#define __NET_CTHREADHPP__

namespace NET
{
	class CThread : public CObject
	{
	public:// TODO: define const here
        enum EPolicy
        {
            SCHED_OTHER = 0,
            SCHED_BATCH,
            SCHED_IDLE,
            SCHED_FIFO,
            SCHED_RR
        };

	private:// TODO: define const here

	public:// TODO: define embed class or struct or enum here

	private:// TODO: define embed class or struct or enum here

	public:// TODO: define your public method here
        CThread();
        virtual ~CThread();

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
        //const UINT*						affinity();
        BOOLEAN								setAffinity( const UINT[]& cpus );

        CThread(CThread&) = delete;
        CThread(const CThread&) = delete;
        CThread& operator=(const CThread&) = delete;

	protected:// TODO: define your protected method here
        virtual void mainLoop(void* arg)
        {
            UNUSED(arg);
            while ( !m_bStop ) {
                ::std::this_thread::sleep_for( ::std::chrono::milliseconds( 1000 ) );
            }
        }

	private:// TODO: define your private method here
        INT									transformPolicy( EPolicy policy );
		
	protected:// property
        BOOLEAN 				m_bStop;
        EPolicy					m_ePolicy;
        UINT 					m_iPriority;

	private:// property
        STRING			        m_strName;
        ::std::thread 			m_thread;
	};
}

#endif  // __NET_CTHREADHPP__