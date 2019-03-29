#ifndef __TYPEDEFINEHPP__
#define __TYPEDEFINEHPP__

namespace NET
{
	typedef 	char 			CHAR;
	typedef		int8_t			INT8;
	typedef		uint8_t			UINT8;
	typedef		int16_t			INT16;
	typedef		uint16_t		UINT16;
	typedef		int32_t			INT;
	typedef		uint32_t		UINT;
	typedef		int64_t			INT64;
	typedef		uint64_t		UINT64;

	typedef 	bool			BOOLEAN;
	typedef 	float			FLOAT;
	typedef 	double			DOUBLE;

	typedef 	UINT8			BYTE;
	typedef 	INT64			LONG;
	typedef		UINT64			ULONG;
	typedef 	INT64			LLONG;
	typedef		UINT64			ULLONG;
	typedef 	::std::string	STRING;

	typedef 	::std::queue	SQUEUE;
	typedef 	::std::stack	SSTACK;
	typedef 	::std::map		SMAP;
	typedef 	::std::mutex	SMUTEX;

#ifndef TRUE
#define TRUE					true
#endif

#ifndef FALSE
#define FALSE					false
#endif

#ifndef STRING_NULL
#define STRING_NULL				""
#endif

#ifndef NULL
#define NULL 					0
#endif

#define MIN(x, y) 				( (x) < (y) ? (x) : (y) )
#define MAX(x, y) 				( (x) > (y) ? (x) : (y) )

#define UNUSED(X) 				(void)(X)

#define SYSTEM_VERSION			"0.2"
#define RELEASE_TIME			"2018-09-13"

enum {
    LOG_FATAL,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_TRACE,
} curent_log_level = LOG_ERROR;

#define STR_TMP(x) #x
#define STR(x) STR_TMP(x)
#define LOG(level, format, ...) do {                                  \
  if (level <= curent_log_level) {                                    \
    fprintf(stderr, __FILE__ ":" STR(__LINE__) ": "                   \
    format "\n", ##__VA_ARGS__);                                      \
  }                                                                   \
} while(0)

#define DIE(format, ...) do {                                         \
  fprintf(stderr, __FILE__ ":" STR(__LINE__) ":"                      \
  format "\n", ##__VA_ARGS__);                                        \
  exit(0);                                                            \
} while(0)

#ifdef __DEBUG__
#include <assert.h>
#define assert_m( x, m )        do { UNUSED( m ); assert( x ); } while(0)
#define assert_r( x, r )        do { UNUSED( r ); assert( x ); } while(0)
#define assert_mr( x, m, r )    do { UNUSED( m ); UNUSED( r ); assert( x ); } while(0)
#else
#define assert( x ) \
            do {\
                if ( !( x ) ) \
                    return; \
            } while(0)       
#define assert_m( x, m ) \
            do {\
                if ( !( x ) ) {\
                    perror( m ); \
                    return; \
                } \
            } while(0)
#define assert_r( x, r ) \
            do {\
                if ( !( x ) ) \
                    return ( r ); \
            } while(0)          
#define assert_mr( x, m, r ) \
            do {\
                if ( !( x ) ) {\
                    perror( m ); \
                    return ( r ); \
                } \
            } while(0)
#endif  // __ASSERT__
}

#endif
