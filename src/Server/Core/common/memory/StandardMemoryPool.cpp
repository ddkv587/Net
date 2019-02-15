#include "NET_Core.hpp"

namespace NET
{
    StandardMemoryPool::StandardMemoryPool( UINT64 size, BOOLEAN boundCheck )
    {
        if ( boundCheck ) m_boundsCheck = 1;

        m_poolMemory = ::new CHAR[ size ];
        m_freePoolSize = size - sizeof(Chunk);
        m_totalPoolSize = size;

        if ( m_trashOnCreation )
            memset( m_poolMemory, s_trashOnCreation, size );

        if ( m_boundsCheck )
        {
            m_freePoolSize -= s_boundsCheckSize * 2;

            Chunk freeChunk( size - sizeof( Chunk ) - 2 * s_boundsCheckSize );
            freeChunk.write( m_poolMemory + s_boundsCheckSize );
            memcpy( m_poolMemory, s_startBound, s_boundsCheckSize );
            memcpy( m_poolMemory + size - s_boundsCheckSize , s_endBound, s_boundsCheckSize );
        }
        else
        {
            Chunk freeChunk( size - sizeof( Chunk ) );
            freeChunk.write( m_poolMemory );
        }
    }

    void* StandardMemoryPool::allocate( UINT64 size )
    {
        UINT64 requiredSize = size + sizeof( Chunk );
        if ( m_boundsCheck )
            requiredSize += s_boundsCheckSize * 2;

        Chunk* block = (Chunk*)( m_boundsCheck == 1 ? m_poolMemory + s_boundsCheckSize : m_poolMemory );
        
        while ( block ) {
            if ( block->m_free && block->m_userdataSize >= requiredSize ) break;
            block = block->m_next;
        }
        if ( !block ) return NULL;

        BYTE* blockData = (BYTE*)block;

        UINT64 freeUserDataSize = block->m_userdataSize - requiredSize;
        if ( freeUserDataSize > s_minFreeBlockSize) {
            // division
            Chunk freeBlock( freeUserDataSize );
            freeBlock.m_next = block->m_next;
            freeBlock.m_prev = block;
            freeBlock.write( blockData + requiredSize );
            if ( freeBlock.m_next )
                freeBlock.m_next->m_prev = (Chunk*)(blockData + requiredSize);
            if ( m_boundsCheck )
                ::memcpy( blockData + requiredSize - s_boundsCheckSize, s_startBound, s_boundsCheckSize );
            block->m_next = (Chunk*)(blockData + requiredSize);
            block->m_userdataSize = size;
        }

        m_freePoolSize -= block->m_userdataSize;
        block->m_free = false;

        if ( m_boundsCheck ) {
            ::memcpy( blockData - s_boundsCheckSize, s_startBound, s_boundsCheckSize );
            ::memcpy( blockData + sizeof( Chunk ) + block->m_userdataSize, s_endBound, s_boundsCheckSize );
        }

        if ( m_trashOnAlloc )
            ::memset( blockData + sizeof( Chunk ), s_trashOnAllocSignature, block->m_userdataSize );
        
        return ( blockData + sizeof( Chunk ) );
    }

    void StandardMemoryPool::free( void* ptrc )
    {
        if ( !ptr ) return;
        Chunk* block = (Chunk*)( (BYTE*)ptr - sizeof(Chunk) );
        ASSERT( block->m_free == false, "This block is already free" );
        if ( block->m_free ) return;

        UINT64 fullBlockSize    = block->m_userdataSize + sizeof(Chunk) + ( m_boundsCheck == 1 ? s_boundsCheckSize * 2 : 0 );
       
        // merge prev
        if ( block->m_prev && block->m_prev->m_free ) {        
            fullBlockSize       += m_boundsCheck == 1 ? 
                                        block->m_prev->m_userdataSize + sizeof(Chunk) + s_boundsCheckSize * 2 : 
                                        block->m_prev->m_userdataSize + sizeof(Chunk);

            block->m_prev->next = block->next;
            block               = block->m_prev;
        }

        // merge next
        if ( block->m_next && block->m_next->m_free ) {
            fullBlockSize       += m_boundsCheck == 1 ? 
                                        block->m_next->m_userdataSize + sizeof(Chunk) + s_boundsCheckSize * 2 : 
                                        block->m_next->m_userdataSize + sizeof(Chunk);

            block->next         = block->next->next;
        }

        UINT64 freeUserDataSize = fullBlockSize - sizeof(Chunk) - ( m_boundsCheck == 1 ) ? s_boundsCheckSize * 2 : 0;

        BYTE* freeBlockStart    = ( BYTE* )block;
        if ( m_trashOnFree )
            ::memset( freeBlockStart + sizeof(Chunk), s_trashOnFreeSignature, freeUserDataSize );
        block->m_userDataSize   = freeUserDataSize;
        block->m_free           = TRUE;
    }

    BOOLEAN StandardMemoryPool::integrityCheck() const
    {
        if ( m_boundsCheck == 1 ) {
            Chunk* temp = (Chunk*)( m_poolMemory + s_boundsCheckSize );

            while ( temp != NULL ) {
                if ( ::memcmp( ((BYTE*)temp ) - s_boundsCheckSize, s_startBound, s_boundsCheckSize) != 0 ) return FALSE; 
                if ( ::memcmp( ((BYTE*)temp ) + sizeof(Chunk) + temp->m_userdataSize, s_endBound, s_boundsCheckSize) != 0 ) return FALSE; 

                temp = temp->m_next;
            }
        }
        return TRUE;
    }

    void StandardMemoryPool::dumpToFile(const STRING& fileName, const UINT64 itemsPerLine) const
    {
        ;
    }

    inline void* StandardMemoryPool::merge( void* ptrDst, void* ptrSrc )
    {
        // TODO
        assert_r( NULL != ptrDst && NULL != ptrSrc, NULL );
        Chunk* blockSrc = (Chunk*)( (BYTE*)ptrSrc - sizeof(Chunk) );
        Chunk* blockDst = (Chunk*)( (BYTE*)ptrDst - sizeof(Chunk) );
        return ptrDst;
    }
}