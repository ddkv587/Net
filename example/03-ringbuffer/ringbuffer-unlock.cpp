#include <memory>
#include <atomic>
#include <thread>

template<class T>
class RingBuffer
{
    public:
        enum ENodeStatus
        {
            NS_INVALID      = 0x00,
            NS_READABLE     = 0x01,
            NS_WRITEABLE    = 0x02,
            NS_PENDING      = 0x04,
        };

    public:
        RingBuffer( ::std::size_t size );
        virtual ~RingBuffer();

        bool                put( const T& );
        bool                put( const T* const );
        T&                  get();
        
        void                reset();
        void                resize( ::std::size_t size );

        bool                empty();
        bool                full();

        ::std::size_t       size()  const;
        ::std::size_t       capacity()  const;

    protected:
        virtual void        optimisticBuffer( ::std::size_t size );

        virtual void        waitForFull();
        virtual void        waitForEmpty();

    private:
        ::std::size_t               m_front;
        ::std::size_t               m_rear;
        ::std::size_t               m_size;
        ::std::size_t               m_mask;

        ::std::unique_ptr<T[]>              m_circularBuffer;
        ::std::unique_ptr<ENodeStatus[]>    m_statusBuffer;
};

template<class T>
RingBuffer<T>::RingBuffer( ::std::size_t size )
    : m_front( 0 )
    , m_rear( 0 )
    , m_size( 0 )
{
    optimisticBuffer( size );
}

template<class T>
RingBuffer<T>::~RingBuffer()
{
    ;
}

template<class T>
bool RingBuffer<T>::put( const T& node )
{
    ::std::size_t pos = m_front;

    while ( !__sync_bool_compare_and_swap( m_statusBuffer[pos], NS_WRITEABLE, NS_PENDING ) )
    {
        pos = ( ++pos ) & ( ~m_mask );

        int count = 0;
        while ( ( ( pos + 1 ) & ( ~m_mask ) ) == m_rear )
        {
            if ( count == 10 )  return false;

            waitForFull();
            ++count;
        }
    }

    m_circularBuffer[pos]   = node;
    m_statusBuffer[pos]     = NS_READABLE;

    // m_front = m_front < pos ? pos + 1 : m_front
    size_t cur;
    do {
        cur = m_front;
        if ( cur >= ( pos + 1 ) )   break;
    } while ( !__sync_bool_compare_and_swap( m_front, cur, pos + 1 ) );

    return true;
}

template<class T>
bool RingBuffer<T>::put( const T* const )
{
    return false;
}

template<class T>
T& RingBuffer<T>::get()
{
    ::std::size_t pos = m_rear;

    while ( !__sync_bool_compare_and_swap( m_statusBuffer[pos], NS_READABLE, NS_PENDING ) )
    {
        pos = ( ++pos ) & ( ~m_mask );

        int count = 0;
        while ( pos == m_front )
        {
            if ( count == 10 )  return T();

            waitForEmpty();
            ++count;
        }
    }

    auto ret = m_circularBuffer[ pos ];
    m_statusBuffer[pos]     = NS_WRITEABLE;

    // m_rear = m_rear < pos ? pos + 1 : m_rear
    size_t cur;
    do {
        cur = m_rear;
        if ( cur >= ( pos + 1 ) )   break;
    } while ( !__sync_bool_compare_and_swap( m_rear, cur, pos + 1 ) );

    return ret;
}

template<class T>
void RingBuffer<T>::reset()
{
    m_circularBuffer.reset( new T[m_mask] );
    m_statusBuffer.reset( new ENodeStatus[m_mask] );

    for ( ::std::size_t index = 0; index < m_mask; ++index )
    {
        m_statusBuffer[index] = NS_WRITEABLE;
    }
}

template<class T>
void RingBuffer<T>::resize( ::std::size_t size )
{
    optimisticBuffer( size );
}

template<class T>
bool RingBuffer<T>::empty()
{
    return ( m_rear == m_front );
}

template<class T>
bool RingBuffer<T>::full()
{
    return ( ( ( m_front + 1 ) & ( ~m_mask ) ) == m_rear );
}

template<class T>
::std::size_t RingBuffer<T>::size()  const
{
    return ( m_front >= m_rear ? m_front - m_rear : m_mask + m_front - m_rear );
}

template<class T>
::std::size_t RingBuffer<T>::capacity()  const
{
    return m_mask;
}

template<class T>
void RingBuffer<T>::optimisticBuffer( ::std::size_t size )
{
    m_mask = 0x01;

    while ( m_mask <= size )
    {
        m_mask <<= 1;
    }

    m_circularBuffer.reset( new T[m_mask] );
    m_statusBuffer.reset( new ENodeStatus[m_mask] );

    for ( ::std::size_t index = 0; index < m_mask; ++index )
    {
        m_statusBuffer[index] = NS_WRITEABLE;
    }
}

template<class T>
void RingBuffer<T>::waitForFull()
{
    ::std::this_thread::yield();
}

template<class T>
void RingBuffer<T>::waitForEmpty()
{
    ::std::this_thread::yield();
}
