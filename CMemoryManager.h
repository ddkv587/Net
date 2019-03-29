#ifndef __CMEMORYMANAGERH__
#define __CMEMORYMANAGERH__

#include <stddef.h>
#include <mutex>

namespace MemoryTrace
{
    namespace MemoryManager
    {
        struct tagUnitNode
        {
            size_t          sign;
            size_t          offset;
            size_t          size;
            void*           pData;
            tagUnitNode*    pPrev;
            tagUnitNode*    pNext;

            bool operator==(const tagUnitNode& pDst)
            {
                return ( ( pDst.sign == sign ) && ( pDst.offset == offset ) && 
                         ( pDst.size == size ) && ( pDst.pData == pData ) &&
                         ( pDst.pPrev == pPrev ) && ( pDst.pNext == pNext ) );
            }
        };

        struct tagUnitManager
        {
            size_t          totalSize;
            size_t          availSize;
            size_t          unitCount;
            tagUnitNode     headUnit;

            tagUnitNode*    pCurrent;
        };

        void                makeUnit(tagUnitNode* const, size_t);
        void                appendUnit(tagUnitNode*);
        void                deleteUnit(tagUnitNode*);
        void                check( bool autoDelete = true );
    }; // namespace MemoryManager
   
    namespace mockMemory
    {
        void*               _mockMalloc( size_t size );
        void*               _mockCalloc( size_t nmemb, size_t size );
        void                _mockFree( void* ptr );
    }; // namespace _mockMemory

    typedef void*           (*FUNC_MALLOC)(size_t);
    typedef void*           (*FUNC_CALLOC)(size_t, size_t);
    typedef void*           (*FUNC_REALLOC)(void *, size_t);
    typedef void*           (*FUNC_MEMALIGN)(size_t, size_t);
    typedef void*           (*FUNC_VALLOC)(size_t);
    typedef void            (*FUNC_FREE)(void* );

    void                    TraceInitialize();
    void                    TraceUninitialize();

    void*                   TraceMalloc( size_t size );
    void*                   TraceCalloc( size_t nmemb, size_t size );
    void*                   TraceRealloc( void* ptr, size_t size );
    void*                   TraceMemalign( size_t blocksize, size_t size );
    void*                   TraceValloc( size_t size );
    void                    TraceFree( void* ptr );

    void*                   _impMalloc( size_t size, bool bRecursive = true );
    void*                   _impCalloc( size_t nmemb, size_t size, bool bRecursive = true );
    void*                   _impRealloc( void* ptr, size_t size, bool bRecursive = true );
    void*                   _impMemalign( size_t blocksize, size_t size, bool bRecursive = true );
    void*                   _impValloc( size_t size, bool bRecursive = true );
    void                    _impFree( void* ptr, bool bRecursive = true );
}; // namespace MemoryTrace
#endif