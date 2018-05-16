#ifndef __COMMANDHPP__
#define __COMMANDHPP__

#include <stdlib.h>

namespace NET 
{
	class CMD final
	{
		public:
			static void COPY(STRING srcFile, STRING desFile) 
			{
				STRING strCmd = "cp -rf " + srcFile + " " + desFile;

				system(strCmd.data());
			}
	
			static void REMOVE(STRING srcFile)
			{
				;
			}

			static void CREATE(STRING srcFile)
			{
				;
			}
	};
}

#endif
