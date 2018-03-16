#ifndef __MULTIPLEXHPP__
#define __MULTIPLEXHPP__

namespace NET
{
	class CMultiBase;
#ifdef OS_BSD
	class CMultiKqueue;
#else
	class CMultiEpoll;
	class CMultiSelect;
#endif
}

#include "MultiBase.hpp"

#ifdef OS_BSD
#include <sys/types.h>
#include <sys/event.h>
#include "MultiKqueue.hpp"
#else
#include <sys/epoll.h>
#include "MultiEpoll.hpp"
#include "MultiSelect.hpp"
#endif

#include "TypeDef.hpp"
#include "common.h"

#endif
