#ifndef __MULTISELECTHPP__
#define __MULTISELECTHPP__

#include "IMultiplex.hpp"

namespace NET
{
	class CMultiSelect : IMultiplex
	{
		public:
			CMultiKqueue();
			virtual ~CMultiKqueue();

			int 	addFileEvent(int, int) = 0;
			void	delFileEvent(int, int) = 0;
			int 	eventLoop(EVENT_LOOP*, int) = 0;
	};
}
#endif
