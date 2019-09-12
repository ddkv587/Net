#include "Memory.h"
#include <assert.h>

namespace Net
{
    template<class T, class A>
        CMemory<T, A>& CMemory<T, A>::null()
        {
            if ( !s_pNULL ) {
                s_pNULL = new CMemory(0);
            }
            return *s_pNULL;
        }

    template<class T, class A>
        CMemory<T, A>::CMemory()
        : m_pMemory(nullptr)
        {
            m_pMemory = null().m_pMemory;
            m_pMemory->addRef();
        }

    template<class T, class A>
        CMemory<T, A>::CMemory(const CMemory& src)
        {
            if ( m_pMemory == src.m_pMemory ) 
                return;

            m_pMemory->subRef();
            m_pMemory = src.m_pMemory;
            m_pMemory->addRef();
        }

    template<class T, class A>
        CMemory<T, A>& CMemory<T, A>::operator=(const CMemory& src)
        {
            if ( m_pMemory == src.m_pMemory ) 
                return *this;

            m_pMemory->subRef();
            m_pMemory = src.m_pMemory;
            m_pMemory->addRef();


            return *this;
        }

    template<class T, class A>
        CMemory<T, A>& CMemory<T, A>::operator=(const T& srcData)
        {
            *m_pMemory = srcData;

            return *this;
        }

    template<class T, class A>
        const T& CMemory<T, A>::operator[](unsigned int index) const
        {
            assert( index >= size() );

            return *(m_pMemory + index);
        }

    template<class T, class A>
        T& CMemory<T, A>::operator[](unsigned int index) 
        {
            assert( index >= size() );

            return *(m_pMemory + index);
        }
}
