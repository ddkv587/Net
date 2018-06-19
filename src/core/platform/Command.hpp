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

				INT ret = system( strCmd.data() );
				UNUSED(ret);
			}
	
			static void REMOVE(STRING srcFile)
			{
                STRING strCmd = "rm -rf " + srcFile;
                
				INT ret = system( strCmd.data() );
				UNUSED(ret);
			}

			static void CREATE(STRING srcFile)
			{
                STRING strCmd = "touch " + srcFile;
                
				INT ret = system( strCmd.data() );
				UNUSED(ret);
			}

			static void LIMIT_OPEN_FILE(UINT size)
			{
				STRING strCmd;

				if ( size == 0 ) 
					strCmd = "ulimit -n " + size;
				else
					strCmd = "ulimit -n unlimited";
                
				INT ret = system( strCmd.data() );
				UNUSED(ret);
			}

			static void SYSCTL_MAX_FILE(UINT size)
			{
				STRING strCmd;

				strCmd = "sysctl -w fs.file-max=" + size;
                
				INT ret = system( strCmd.data() );
				UNUSED(ret);
			}
	};
}

#endif
