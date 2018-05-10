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
	typedef 	INT64			POINT;
	typedef 	::std::string	STRING;

#define TRUE	true 
#define FALSE	false

#define MIN(x, y) 	( (x) < (y) ? (x) : (y) )
#define MAX(x, y) 	( (x) > (y) ? (x) : (y) )

#define CHECK_R(X, R) \
	do { if (!(X)) return (R); } while(0)
}

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define handle_error_errno(en, msg) \
	do { errno = en, perror(msg); exit(EXIT_FAILURE); } while(0)

#define UNUSED(X) (void)(X)

#endif
