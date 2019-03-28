#include "CMemoryManager.h"

#include <assert.h>

extern "C"
{
    void* malloc( size_t size )
    {
        return MemoryTrace::TraceMalloc( size );
    }

    void free( void* ptr )
    {
        MemoryTrace::TraceFree( ptr );
    }

    void* calloc( size_t n, size_t len )
    {
        return MemoryTrace::TraceCalloc( n, len );
    }

    void* realloc(void *ptr, size_t size)
    {
        return MemoryTrace::TraceRealloc( ptr, size );
    }

    void* memalign(size_t blocksize, size_t bytes) 
    {
        return MemoryTrace::TraceMemalign( blocksize, bytes );
    }

    void* valloc(size_t size) 
    {
        return MemoryTrace::TraceValloc( size );
    }
}