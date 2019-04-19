#include "CMemoryManager.h"
#include <assert.h>
#include <unistd.h>

static bool bShow = false;
void* operator new( size_t size )   
{ 
	void* p = MemoryTrace::TraceMalloc( size );
    if( bShow ) fprintf( stderr, "call new size: %ld, %p\n", size, p );

    return p;
}

void* operator new[](size_t size ) 
{ 
	void* p = MemoryTrace::TraceMalloc( size );
    if( bShow ) fprintf( stderr, "call new[] size: %ld, %p\n", size, p );
	
    return p;
}

void operator delete( void* ptr )  
{ 
    if( bShow ) fprintf( stderr, "call delete: %p\n", ptr );
	MemoryTrace::TraceFree( ptr );
}

void operator delete[]( void* ptr )    
{ 
    if( bShow ) fprintf( stderr, "call delete[]: %p\n", ptr );
	MemoryTrace::TraceFree( ptr );
}

extern "C"
{
	// void* malloc( size_t size )
	// {  
	//     return MemoryTrace::TraceMalloc( size );
	// }

	// void free( void* ptr )
	// {
	//     MemoryTrace::TraceFree( ptr );
	// }

	// void* calloc( size_t n, size_t len )
	// {
	//     return MemoryTrace::TraceCalloc( n, len );
	// }

	// void* realloc(void *ptr, size_t size)
	// {
	//     return MemoryTrace::TraceRealloc( ptr, size );
	// }

	// void* memalign(size_t blocksize, size_t bytes) 
	// {  
	//     return MemoryTrace::TraceMemalign( blocksize, bytes );
	// }

	// void* valloc(size_t size) 
	// {        
	//     return MemoryTrace::TraceValloc( size );
	// }

	// // int posix_memalign(void **memptr, size_t alignment, size_t size)
	// // {
	// //     return MemoryTrace::TracePosixMemalign( memptr, alignment, size );
	// // }

	// void *aligned_alloc(size_t alignment, size_t size)
	// {
	//     assert(false);
	// }

	// void *pvalloc(size_t size)
	// {
	//     assert(false);
	// }

    void exit(int status)
    {
        bShow = true;
       
        MemoryTrace::TraceUninitialize();

        _exit( status );
    }
}