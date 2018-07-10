#include "NET_Core.hpp"

namespace NET
{
    static ::std::tuple<INT, INT> CThreadBase::range(EPolicy policy)
    {
        return ::std::make_tuple( sched_get_priority_max(getPolicy(policy)), sched_get_priority_min(getPolicy(policy)));
    }

	BOOLEAN CThreadBase::setPriority(INT iPriority, EPolicy policy)
    {
        if ( m_iPriority == iPriority ) return TRUE;

        CHECK_R( 0 == pthread_attr_init (&attr), FALSE);
        return ( 0 == pthread_attr_setschedpolicy() );
    }	

	UINT CThreadBase::priority()
    {

    }

    INT CThreadBase::getPolicy(EPolicy policy)
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

