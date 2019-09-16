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

        void						    start( void* arg = NULL )                       { m_thread = ::std::thread(&CThread::mainLoop, this, std::move(arg)); }
        void 						    stop()                                          { m_bStop = TRUE; }
        cosnt ::std::thread::id 		threadID()                                      { return m_thread.get_id(); }

        inline cosnt STRING& 		    threadName()								    { return m_strName; }
        void 						    threadName( const STRING& strName )			    { m_strName = strName; }

        void                            wait()                                          { m_waitCondition.wait_for( ::std::unique_lock<std::mutex>( m_waitMutex ), std::chrono::milliseconds( 2000 ) ); }
        void                            notify()                                        { m_waitCondition.notify_one(); }

        // ============= priority ===================
        inline EPolicy 				    policy()							            { return m_ePolicy; }
        inline INT 					    priority()							            { return m_iPriority; }
        BOOLEAN 					    priority( INT iPriority, EPolicy policy );

        // ============= affinity ===================
        //const UINT*					affinity();
        BOOLEAN						    affinity( const UINT[]& cpus );

        CThread(CThread&) = delete;
        CThread(const CThread&) = delete;
        CThread& operator=(const CThread&) = delete;

    protected:// TODO: define your protected method here
        virtual void mainLoop(void* arg)
        {
            UNUSED(arg);
            while ( !m_bStop ) {
                wait();
            }
        }

    private:// TODO: define your private method here
        INT					            transformPolicy( EPolicy policy );

    protected:// property
        BOOLEAN                     m_bStop;
        EPolicy                     m_ePolicy;
        UINT                        m_iPriority;

    private:// property
        STRING                      m_strName;
        STHREAD                     m_thread;

        SMUTEX                      m_waitMutex;
        ::std::condition_variable   m_waitCondition;
    };
}

#endif  // __NET_CTHREADHPP__