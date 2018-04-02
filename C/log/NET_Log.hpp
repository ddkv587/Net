#ifndef __NETLOGHPP__
#define __NETLOGHPP__

namespace NET
{
	class CLog;
}

#include <glog/logging.h>
using namespace google;

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "Log.hpp"

extern int errno;

#endif
