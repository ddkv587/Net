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

				UNUSED( system(strCmd.data()) );
			}
	
			static void REMOVE(STRING srcFile)
			{
                STRING strCmd = "rm -rf " + srcFile;
                
                UNUSED( system(strCmd.data() );
			}

			static void CREATE(STRING srcFile)
			{
                STRING strCmd = "touch " + srcFile;
                
                UNUSED( system(strCmd.data()) );
			}
	};
}

#endif
