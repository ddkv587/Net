#ifndef __CMEMORYMANAGERH__
#define __CMEMORYMANAGERH__

#include <stddef.h>

namespace MemoryTrace
{
    class CMemoryManager
    {
    private:
        typedef void* (*FUNC_MALLOC)(size_t);
        typedef void* (*FUNC_CALLOC)(size_t, size_t);
        typedef void* (*FUNC_REALLOC)(void *, size_t);
        typedef void* (*FUNC_MEMALIGN)(size_t, size_t);
        typedef void* (*FUNC_VALLOC)(size_t);
        typedef void (*FUNC_FREE)(void *);

    public:
        static CMemoryManager* getInstance();
        static void            delInstance();

        bool                    isInitialized()         { return m_bInitialized; }
        void*                   TraceMalloc( size_t size );
        void*                   TraceCalloc( size_t nmemb, size_t size );
        void*                   TraceRealloc( void *ptr, size_t size );
        void*                   TraceMemalign( size_t blocksize, size_t bytes );
        void*                   TraceValloc( size_t size );
        void                    TraceFree( void *ptr );
        bool                    TraceInit();

        CMemoryManager();
        virtual ~CMemoryManager();

    protected:
        
    private:
        static CMemoryManager*      s_pInstance;

        bool                        m_bInitialized;
        FUNC_MALLOC                 m_pRealMalloc;
        FUNC_CALLOC                 m_pRealCalloc;
        FUNC_REALLOC                m_pRealRealloc;
        FUNC_MEMALIGN               m_pRealMemalign;
        FUNC_VALLOC                 m_pRealValloc;
        FUNC_FREE                   m_pRealFree;
    };
}
#endif