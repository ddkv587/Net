#include "NET_Core.hpp"

namespace NET
{
    static ::std::tuple<INT, INT> CThreadBase::range(EPolicy policy)
    {
        return ::std::make_tuple( sched_get_priority_max(transformPolicy(policy)), sched_get_priority_min(transformPolicy(policy)));
    }

	BOOLEAN CThreadBase::setPriority(INT iPriority, EPolicy policy)
    {
        if ( m_ePolicy == policy && m_iPriority == iPriority ) return TRUE;

        pthread_attr_t attr;
        struct sched_param sched;

        CHECK_R ( 0 == pthread_attr_init (&attr), FALSE );
        CHECK_R ( 0 == pthread_attr_setschedpolicy(&attr, transformPolicy(policy)), FALSE );
        sched.__sched_priority = iPriority;

        CHECK_R ( 0 == pthread_attr_setschedparam(&attr, &sched), FALSE );

        m_ePolicy   = policy;
        m_iPriority = iPriority;

        return TRUE;
    }	
    
    EPolicy CThreadBase::policy()
    {
        return m_ePolicy;
    }	

	INT CThreadBase::priority()
    {
        return m_iPriority;
    }

    INT CThreadBase::transformPolicy(EPolicy policy)
    {
        switch(policy)
        {
            case SCHED_OTHER:
                return ::SCHED_OTHER;
			case SCHED_BATCH:
                return ::SCHED_BATCH;
			case SCHED_IDLE,
                return ::SCHED_IDLE;
			case SCHED_FIFO,
                return ::SCHED_FIFO;
			case SCHED_RR: 
                return ::SCHED_RR;
        }
    }
    
} // NET

