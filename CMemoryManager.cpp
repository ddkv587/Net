#include <assert.h>
#include <dlfcn.h>
#include <cstdio>
#include <mutex>
#include <functional>
#include <math.h>
#include "CMemoryManager.h"

namespace MemoryTrace
{
    #define DEF_SIZE_UNIT_NODE  sizeof( MemoryManager::tagUnitNode )
    
    #define PTR_UNIT_NODE_HEADER(ptr_unit_data)			        ( MemoryManager::tagUnitNode* )( (char*)ptr_unit_data - DEF_SIZE_UNIT_NODE )
    #define PTR_UNIT_NODE_DATA(ptr_unit_hdr)			        ( void* )( (char*)ptr_unit_hdr + DEF_SIZE_UNIT_NODE )
    #define PTR_OFFSET_NODE_HEADER(ptr_unit_start, offset)		( MemoryManager::tagUnitNode* )( (char*)ptr_unit_start + offset )

    #define UNIT_NODE_MAGIC								        0xFEEF9FF9CDDC9889
    #define MAKE_UNIT_NODE_MAGIC(ptr_unit_hdr)			        ( UNIT_NODE_MAGIC ^ (size_t)ptr_unit_hdr )

    namespace MemoryManager
    {
        static tagUnitManager s_unitManager = \
        { 
            .totalSize          = 0,
            .availSize          = 0,
            .unitCount          = 0,
            .headUnit           = {
                .sign   = 0,
                .offset = 0,
                .size   = 0,
                .pData  = NULL,
                .pPrev  = NULL,
                .pNext  = NULL,
            },
            .pCurrent           = &( s_unitManager.headUnit ),
        };

        static std::mutex s_mutexMemory;

        void makeUnit(tagUnitNode* const pNode, size_t size)
        {
            if ( NULL == pNode ) return;
            
            pNode->sign     = MAKE_UNIT_NODE_MAGIC( pNode );
            pNode->offset   = (size_t)pNode - (size_t)(&s_unitManager);
            pNode->size     = size;
            pNode->pData    = PTR_UNIT_NODE_DATA( pNode );
            pNode->pPrev    = NULL;
            pNode->pNext    = NULL;
        }

        void appendUnit( tagUnitNode* pNode )
        {
            std::lock_guard<std::mutex> lock( s_mutexMemory );
            
            if ( NULL == pNode ) return;
            
            pNode->pPrev = s_unitManager.pCurrent;
            pNode->pNext = NULL;

            assert( NULL != s_unitManager.pCurrent );
            s_unitManager.pCurrent->pNext = pNode;
            s_unitManager.pCurrent = pNode;

            s_unitManager.totalSize += pNode->size;
            s_unitManager.availSize += pNode->size - DEF_SIZE_UNIT_NODE;
            s_unitManager.unitCount += 1;
        }

        bool deleteUnit( tagUnitNode* pNode )
        {
            std::lock_guard<std::mutex> lock( s_mutexMemory );
            
            if ( NULL == pNode )    return true;

            tagUnitNode* pCheckNode = PTR_OFFSET_NODE_HEADER( &s_unitManager, pNode->offset );

            // hook all type of memory request, this must be true
            assert( MAKE_UNIT_NODE_MAGIC( pNode ) == pNode->sign && (*pCheckNode == *pNode ) );
            assert( NULL != s_unitManager.pCurrent );
            if ( s_unitManager.pCurrent == pCheckNode )
                s_unitManager.pCurrent = pCheckNode->pPrev;

            assert ( NULL != pCheckNode->pPrev );
            pCheckNode->pPrev->pNext = pCheckNode->pNext;

            if ( NULL != pCheckNode->pNext )
                pCheckNode->pNext->pPrev = pCheckNode->pPrev;

            return true;
        }

        void check( bool autoDelete )
        {
            tagUnitNode* pNode = s_unitManager.headUnit.pNext;

            tagUnitNode* pCur;
            while ( NULL != pNode ) 
            {
                fprintf( stderr, "unfree addr: %p, size: %ld\n", pNode, pNode->size - DEF_SIZE_UNIT_NODE );
                pCur    = pNode;
                pNode   = pNode->pNext;

                if ( autoDelete ) TraceFree( pCur );
            }
        }
    } // namespace MemoryManager

    namespace mockMemory
    {
        static char             s_mockBuffer[1024 * 1024];
        static size_t           s_mockMallocPos = 0;

        void* _mockMalloc( size_t size )
        {
            assert ( s_mockMallocPos + size < sizeof( s_mockBuffer ) );
            void* ptr = (void*)(s_mockBuffer + s_mockMallocPos);
            s_mockMallocPos += size;

            fprintf( stderr, "mock malloc: %p, size: %ld\n", ptr, size );
            return ptr;
        }

        void* _mockCalloc( size_t nmemb, size_t size )
        {
            void* ptr = _mockMalloc( nmemb * size );
            for ( size_t i=0; i < nmemb * size; ++i )
                *((char*)(ptr) + i ) = '\0';

            fprintf( stderr, "mock calloc: %p, size: %ld\n", ptr, nmemb * size );
            return ptr; 
        }

        void _mockFree( void* ptr )
        {
            ;
        }    
    } // namespace mockMemory
   
    static bool             m_bInitialized      = false;
    static bool             m_bInitializing     = false;
    static FUNC_MALLOC      m_pRealMalloc       = mockMemory::_mockMalloc;
    static FUNC_CALLOC      m_pRealCalloc       = mockMemory::_mockCalloc;
    static FUNC_REALLOC     m_pRealRealloc      = NULL;
    static FUNC_MEMALIGN    m_pRealMemalign     = NULL;
    static FUNC_VALLOC      m_pRealValloc       = NULL;
    static FUNC_FREE        m_pRealFree         = mockMemory::_mockFree;

    static std::mutex       s_mutexInit;

    void TraceInitialize()
    {
        fprintf( stderr, "TraceInit\n" );
        std::lock_guard<std::mutex> lock( s_mutexInit );
        if ( m_bInitialized ) return;

        m_bInitializing = true;
        
        FUNC_MALLOC     tmpMalloc       = (FUNC_MALLOC)dlsym(RTLD_NEXT, "malloc");
        FUNC_CALLOC     tmpCalloc       = (FUNC_CALLOC)dlsym(RTLD_NEXT, "calloc");
        FUNC_REALLOC    tmpRealloc      = (FUNC_REALLOC)dlsym(RTLD_NEXT, "realloc");
        FUNC_MEMALIGN   tmpMemalign     = (FUNC_MEMALIGN)dlsym(RTLD_NEXT, "memalign");
        FUNC_VALLOC     tmpValloc       = (FUNC_VALLOC)dlsym(RTLD_NEXT, "valloc");
        FUNC_FREE       tmpFree         = (FUNC_FREE)dlsym(RTLD_NEXT, "free");

        assert( !( NULL == tmpMalloc || NULL == tmpCalloc || NULL == tmpRealloc || 
              NULL == tmpMemalign || NULL == tmpValloc || NULL == tmpFree ) );

        m_pRealMalloc       = tmpMalloc;
        m_pRealCalloc       = tmpCalloc;
        m_pRealRealloc      = tmpRealloc;
        m_pRealMemalign     = tmpMemalign;
        m_pRealValloc       = tmpValloc;
        m_pRealFree         = tmpFree;

        m_bInitialized  = true;
        m_bInitializing = false;   
    }

    void TraceUninitialize()
    {
        MemoryManager::check(false);
    }

    static int s_no_hook    = 0;
    void* TraceMalloc( size_t size )
    {
        if ( m_bInitializing ) return mockMemory::_mockMalloc(size);
        
        if ( !m_bInitialized )  TraceInitialize();

        void* p = _impMalloc( size, __sync_fetch_and_add( &s_no_hook, 1 ) );
        __sync_fetch_and_sub( &s_no_hook, 1 );
        return p;
    }

    void* TraceCalloc( size_t nmemb, size_t size )
    { 
        if ( m_bInitializing ) return mockMemory::_mockCalloc(nmemb, size);
        
        if ( !m_bInitialized )  TraceInitialize();

        void* p = _impCalloc( nmemb, size, __sync_fetch_and_add( &s_no_hook, 1 ) );
        __sync_fetch_and_sub( &s_no_hook, 1 );
        return p;
    }

    void* TraceRealloc( void *ptr, size_t size )
    {
        if ( !m_bInitialized )  TraceInitialize();

        void* p = _impRealloc( ptr, size, __sync_fetch_and_add( &s_no_hook, 1 ) );
        __sync_fetch_and_sub( &s_no_hook, 1 );
        return p;
    }

    void* TraceMemalign( size_t blocksize, size_t bytes )
    {
        if ( !m_bInitialized )  TraceInitialize();

        void* p = _impMemalign( blocksize, bytes, __sync_fetch_and_add( &s_no_hook, 1 ) );
        __sync_fetch_and_sub( &s_no_hook, 1 );
        return p;
    }
    
    void* TraceValloc( size_t size )
    {
        if ( !m_bInitialized )  TraceInitialize();

        void* p = _impValloc( size, __sync_fetch_and_add( &s_no_hook, 1 ) );
        __sync_fetch_and_sub( &s_no_hook, 1 );
        return p;
    }

    void TraceFree( void *ptr )
    {
        if ( m_bInitializing ) return mockMemory::_mockFree(ptr);
        
        if ( !m_bInitialized )  TraceInitialize();

        _impFree( ptr, __sync_fetch_and_add( &s_no_hook, 1 ) );
        __sync_fetch_and_sub( &s_no_hook, 1 );
    }

    void* _impMalloc( size_t size, bool bRecursive )
    {
        MemoryManager::tagUnitNode* pNode = (MemoryManager::tagUnitNode*)m_pRealMalloc( size + DEF_SIZE_UNIT_NODE );

        if ( NULL == pNode ) return NULL;

        MemoryManager::makeUnit( pNode, size + DEF_SIZE_UNIT_NODE );
        MemoryManager::appendUnit( pNode );

#ifdef _DEBUG
        if ( !bRecursive )
            fprintf(stderr, "===malloc: %p, size: %ld, real: %ld\n", pNode, pNode->size, pNode->size - DEF_SIZE_UNIT_NODE);
#endif
        return PTR_UNIT_NODE_DATA( pNode );
    }

    void* _impCalloc( size_t nmemb, size_t size, bool bRecursive )
    {
        nmemb = ceil( (double)( size * nmemb + DEF_SIZE_UNIT_NODE ) / (double)size );
        MemoryManager::tagUnitNode* pNode = (MemoryManager::tagUnitNode*)m_pRealCalloc( nmemb, size );

        if ( NULL == pNode ) return NULL;
        
        MemoryManager::makeUnit( pNode, nmemb * size );
        MemoryManager::appendUnit( pNode );

#ifdef _DEBUG
        if ( !bRecursive )
            fprintf(stderr, "===calloc: %p, size: %ld, real: %ld\n", pNode, pNode->size, pNode->size - DEF_SIZE_UNIT_NODE);
#endif
        return PTR_UNIT_NODE_DATA( pNode );
    }

    void* _impRealloc( void *ptr, size_t size, bool bRecursive )
    {
        //if ( NULL != ptr ) _impFree( ptr, true );
        MemoryManager::tagUnitNode* pNode = (MemoryManager::tagUnitNode*)m_pRealRealloc( NULL, size + DEF_SIZE_UNIT_NODE );
        
        if ( NULL == pNode ) return NULL;
     
        MemoryManager::makeUnit( pNode, size + DEF_SIZE_UNIT_NODE );
        MemoryManager::appendUnit( pNode );

#ifdef _DEBUG
        if ( !bRecursive )
            fprintf(stderr, "===realloc: %p, size: %ld, real: %ld\n", pNode, pNode->size, pNode->size - DEF_SIZE_UNIT_NODE);
#endif
        return PTR_UNIT_NODE_DATA( pNode );
    }

    void* _impMemalign( size_t blocksize, size_t size, bool bRecursive )
    {
        MemoryManager::tagUnitNode* pNode = (MemoryManager::tagUnitNode*)m_pRealMemalign( blocksize, size + DEF_SIZE_UNIT_NODE );

        if ( NULL == pNode ) return NULL;
        
        MemoryManager::makeUnit( pNode, size + DEF_SIZE_UNIT_NODE );
        MemoryManager::appendUnit( pNode );

#ifdef _DEBUG
        if ( !bRecursive )
            fprintf(stderr, "===memalign: %p, size: %ld, real: %ld\n", pNode, pNode->size, pNode->size - DEF_SIZE_UNIT_NODE);
#endif
        return PTR_UNIT_NODE_DATA( pNode );
    }

    void* _impValloc( size_t size, bool bRecursive )
    {
        MemoryManager::tagUnitNode* pNode = (MemoryManager::tagUnitNode*)m_pRealValloc( size + DEF_SIZE_UNIT_NODE );

        if ( NULL == pNode ) return NULL;
        
        MemoryManager::makeUnit( pNode, size + DEF_SIZE_UNIT_NODE );
        MemoryManager::appendUnit( pNode );

#ifdef _DEBUG
        if ( !bRecursive )
            fprintf(stderr, "===valloc: %p, size: %ld, real: %ld\n", pNode, pNode->size, pNode->size - DEF_SIZE_UNIT_NODE);
#endif
        return PTR_UNIT_NODE_DATA( pNode );
    }

    void _impFree( void* ptr, bool bRecursive )
    {
        if ( NULL == ptr ) return;
        MemoryManager::tagUnitNode* pNode = PTR_UNIT_NODE_HEADER( ptr );

#ifdef _DEBUG       
        if ( !bRecursive )
            fprintf(stderr, "===free: %p, size: %ld, real: %ld\n", pNode, pNode->size, pNode->size - DEF_SIZE_UNIT_NODE);
#endif        
        MemoryManager::deleteUnit( pNode );
        m_pRealFree( pNode );
    }
}
