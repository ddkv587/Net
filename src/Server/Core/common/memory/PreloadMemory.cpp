#include "CMemoryManager.h"

extern "C"
{
    void* malloc( size_t size )
    {
        return MemoryTrace::CMemoryManager::getInstance()->TraceMalloc( size );
    }

    void free( void* ptr )
    {
        MemoryTrace::CMemoryManager::getInstance()->TraceFree( ptr );
    }

    void* calloc( size_t n, size_t len )
    {
        return MemoryTrace::CMemoryManager::getInstance()->TraceCalloc( n, len );
    }
}