#include <assert.h>
#include <dlfcn.h>
#include <cstdio>
#include "CMemoryManager.h"

namespace MemoryTrace
{
    static int s_no_hook = 0;

    //CMemoryManager* CMemoryManager::s_pInstance = NULL;
    static CMemoryManager s_Instance;

    CMemoryManager* CMemoryManager::getInstance()
    {
        // if ( NULL == s_pInstance ) 
        // {
        //     s_pInstance = new CMemoryManager();
        // }
        // return s_pInstance;
        if ( !s_Instance.isInitialized() ) { s_Instance.TraceInit(); }
        return &s_Instance;
    }

    void CMemoryManager::delInstance()
    {

    }
    
    void* CMemoryManager::TraceMalloc( size_t size )
    {
        assert( NULL != m_pRealMalloc );

        if ( __sync_fetch_and_add( &s_no_hook, 1 ) ) 
        {
            __sync_fetch_and_sub( &s_no_hook, 1 );
            return m_pRealMalloc( size );
        }   
        void *p = m_pRealMalloc( size );
        if ( size > 10000 )
            fprintf( stderr, "malloc(%d) = %p\n", size, p );

        __sync_fetch_and_sub( &s_no_hook, 1 );

        return p;
    }

    void* CMemoryManager::TraceCalloc( size_t nmemb, size_t size )
    {
        assert( NULL != m_pRealCalloc );
        
        if ( __sync_fetch_and_add( &s_no_hook, 1 ) ) 
        {
            __sync_fetch_and_sub( &s_no_hook, 1 );
            return m_pRealCalloc( nmemb, size );
        }   
        void *p = m_pRealCalloc( nmemb, size );
        __sync_fetch_and_sub( &s_no_hook, 1 );

        return p;
    }

    void* CMemoryManager::TraceRealloc( void *ptr, size_t size )
    {
        assert( NULL != m_pRealRealloc );

        if ( __sync_fetch_and_add( &s_no_hook, 1 ) ) 
        {
            __sync_fetch_and_sub( &s_no_hook, 1 );
            return m_pRealRealloc( ptr, size );
        }   
        void *p = m_pRealRealloc( ptr, size );
        __sync_fetch_and_sub( &s_no_hook, 1 );

        return p;
    }

    void* CMemoryManager::TraceMemalign( size_t blocksize, size_t bytes )
    {
        assert( NULL != m_pRealMemalign );

        if ( __sync_fetch_and_add( &s_no_hook, 1 ) ) 
        {
            __sync_fetch_and_sub( &s_no_hook, 1 );
            return m_pRealMemalign( blocksize, bytes );
        }   
        void *p = m_pRealMemalign(  blocksize, bytes  );
        __sync_fetch_and_sub( &s_no_hook, 1 );

        return p;
    }
    
    void* CMemoryManager::TraceValloc( size_t size )
    {
        assert( NULL != m_pRealValloc );

        if ( __sync_fetch_and_add( &s_no_hook, 1 ) ) 
        {
            __sync_fetch_and_sub( &s_no_hook, 1 );
            return m_pRealValloc( size );
        }   
        void *p = m_pRealValloc( size );
        __sync_fetch_and_sub( &s_no_hook, 1 );

        return p;
    }

    void CMemoryManager::TraceFree( void *ptr )
    {
        assert( NULL != m_pRealFree );

        if ( __sync_fetch_and_add( &s_no_hook, 1 ) ) 
        {
            __sync_fetch_and_sub( &s_no_hook, 1 );
            m_pRealFree( ptr );
            return;
        }   
        m_pRealFree( ptr );
        __sync_fetch_and_sub( &s_no_hook, 1 );
    }

    CMemoryManager::CMemoryManager()
        : m_bInitialized(false)
        , m_pRealMalloc(NULL)
        , m_pRealCalloc(NULL)
        , m_pRealRealloc(NULL)
        , m_pRealMemalign(NULL)
        , m_pRealValloc(NULL)
        , m_pRealFree(NULL)
    {
        ;
    }

    CMemoryManager::~CMemoryManager()
    {
        ;
    }

    bool CMemoryManager::TraceInit()
    {
        m_pRealMalloc         = dlsym(RTLD_NEXT, "malloc");
        m_pRealCalloc         = dlsym(RTLD_NEXT, "calloc");
        m_pRealRealloc        = dlsym(RTLD_NEXT, "realloc");
        m_pRealFree           = dlsym(RTLD_NEXT, "free");
        m_pRealMemalign       = dlsym(RTLD_NEXT, "memalign");
        m_pRealValloc         = dlsym(RTLD_NEXT, "valloc");

        if ( !m_pRealMalloc || !m_pRealCalloc || !m_pRealRealloc || !m_pRealFree ||
            !m_pRealMemalign || !m_pRealValloc )
        {
            fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
            return false;
        }
        m_bInitialized = true;
        return true;
    }
}
