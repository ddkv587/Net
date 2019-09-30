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
        T                   get();
        
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
        ::std::atomic_size_t        m_front;
        ::std::atomic_size_t        m_rear;
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
    ::std::size_t pos = m_front.load();

    while ( !__sync_bool_compare_and_swap( &( m_statusBuffer[ pos & ( ~m_mask ) ] ), NS_WRITEABLE, NS_PENDING ) ) {
        int count = 0;

    }

    m_circularBuffer[ pos & ( ~m_mask ) ]   = node;
    m_statusBuffer[ pos & ( ~m_mask ) ]     = NS_READABLE;

    // m_front = m_front < pos ? pos + 1 : m_front
    ::std::size_t cur;

    do {
        cur = m_front.load();
        if ( cur >= ( pos + 1 ) ) {
            printf( " === thread %ld === modify out!!!!!!!!!!\n", ::std::this_thread::get_id() );
            break;
        }
    } while ( !m_front.compare_exchange_weak( cur, pos + 1 ) );

    printf( "put pos: %ld, front: %ld, rear: %ld\n", pos, m_front.load(), m_rear.load() );

    return true;
}

template<class T>
bool RingBuffer<T>::put( const T* const )
{
    return false;
}

template<class T>
T RingBuffer<T>::get()
{
    ::std::size_t pos = m_rear;

    while ( !__sync_bool_compare_and_swap( &( m_statusBuffer[pos] ), NS_READABLE, NS_PENDING ) )
    {
        int count = 0;
        while ( pos == m_front.load() )
        {
            if ( count == 10 )  return T();

            waitForEmpty();
            ++count;
        }
    }

    auto ret = m_circularBuffer[ pos ];
    m_statusBuffer[pos]     = NS_WRITEABLE;

    // m_rear = m_rear < pos ? pos + 1 : m_rear
    ::std::size_t cur;

    do {
        cur = m_rear.load();
        if ( cur >= ( pos + 1 ) )   
        {
            printf( " === thread %ld === modify out!!!!!!!!!!\n", ::std::this_thread::get_id() );
            break;
        }
    } while ( !m_rear.compare_exchange_weak( cur, pos + 1 ) );

    printf( "get pos: %ld, front: %ld, rear: %ld\n", pos, m_front.load(), m_rear.load() );

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
    return ( m_rear.load() == m_front.load() );
}

template<class T>
bool RingBuffer<T>::full()
{
    return ( ( m_front.load() + 1 ) == m_rear.load() );
}

template<class T>
::std::size_t RingBuffer<T>::size()  const
{
    return ( m_front.load() - m_rear.load() );
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

    printf( "optimisticBuffer: %ld\n", m_mask );
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

int main(int argc, char const *argv[])
{
    const static int count = 100;
    static RingBuffer<int> buffer( 100 );

    ::std::thread* arrThread[count];

    for ( int index = 0; index < count; ++index ) {
        arrThread[index] = new ::std::thread(
            [] ( int start ) {
                while ( 1 )
                {
                    if ( buffer.full() )
                    {
                        printf("full\n");
                    }
                    else
                    {
                        printf( "=== thread %ld === put: %d, size: %ld\n", ::std::this_thread::get_id(), buffer.size() );
                        buffer.put( start++ );
                    }
                    std::this_thread::sleep_for(::std::chrono::milliseconds(500));
                }
            }, index * 10);
    }

    // ::std::thread t3(
    //     [] {
    //         int index = 0;
    //         while ( 1 )
    //         {
    //             if (buffer.empty())
    //             {
    //                 printf("empty\n");
    //             }
    //             else
    //             {
    //                 printf("get: %d\n", buffer.get());
    //             }
    //             std::this_thread::sleep_for(::std::chrono::milliseconds(2000));
    //         }
    //     });
    for ( int index = 0; index < count; ++index ) {
        arrThread[index]->join();
    }
    // t2.join();
    //t3.join();

    return 0;
}

