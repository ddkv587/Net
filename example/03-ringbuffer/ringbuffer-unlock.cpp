#include <memory>
#include <atomic>
#include <thread>
#include <vector>
#include <mutex>

template<class T>
class RingBuffer
{
    public:
        enum ENodeStatus
        {
            NS_INVALID          = 0x00,
            NS_READABLE         = 0x01,
            NS_READPENDING      = 0x02,
            NS_WRITEABLE        = 0x04,
            NS_WRITEPENDING     = 0x08,
        };

    public:
        RingBuffer( ::std::size_t size );
        virtual ~RingBuffer();

        bool                put( const T& );
        bool                put( const T* const );
        T                   get();
        
        void                reset();
        void                resize( ::std::size_t size );

        inline bool         empty();
        inline bool         full();

        ::std::size_t       size()  const;
        ::std::size_t       capacity()  const;

    protected:
        virtual void        optimisticBuffer( ::std::size_t size );

        virtual bool        waitForFull();
        virtual bool        waitForEmpty();

    private:
        inline size_t       locate( ::std::size_t cursor );

    private:
        ::std::atomic_size_t        m_front;
        ::std::atomic_size_t        m_rear;
        ::std::size_t               m_size;
        ::std::size_t               m_mask;
        ::std::mutex                m_mutex;

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
    // local the avaliable place
    ::std::size_t pos = 0;
    {
        do {
            while ( full() ) {
                if ( !waitForFull() ) return false;
            }
            pos = m_front.load();
            
            if ( m_statusBuffer[ locate(pos) ] == NS_READPENDING ) {
                continue;   // wait for read complete
            } else {
                // update m_front
                ::std::size_t cur = 0;
                do {
                    cur = m_front.load();
                    if ( cur >= ( pos + 1 ) ) {
                        fprintf( stderr, "=== thread %zd === modify out!!!!!!!!!!\n", ::std::hash<std::thread::id>{}( ::std::this_thread::get_id() ) );
                        break;
                    }
                } while ( !m_front.compare_exchange_weak( cur, pos + 1 ) );
            }
        } while ( !__sync_bool_compare_and_swap( &m_statusBuffer[ locate(pos) ], NS_WRITEABLE, NS_WRITEPENDING ) );
    }

    fprintf( stderr, "put pos: %ld, front: %ld, rear: %ld\n", pos, m_front.load(), m_rear.load() );

    // update data
    m_circularBuffer[ locate(pos) ]   = node;
    m_statusBuffer[ locate(pos) ]     = NS_READABLE;

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
    // local the avaliable place
    ::std::size_t pos = 0;
    {
        do {
            while ( empty() ) {
                if ( !waitForEmpty() ) return false;
            }
            pos = m_rear.load();

            if ( m_statusBuffer[ locate(pos) ] == NS_WRITEPENDING ) {
                continue;   // wait for write complete
            } else {
                // update m_rear
                ::std::size_t cur = 0;
                do {
                    cur = m_rear.load();
                    if ( cur >= ( pos + 1 ) ) {
                        fprintf( stderr, "=== thread %zd === modify out!!!!!!!!!!\n", ::std::hash<std::thread::id>{}( ::std::this_thread::get_id() ) );
                        break;
                    }
                    if ( cur == m_front.load() ) {
                        fprintf( stderr, "=== rear catch front !!!!!!!!!!\n" );
                        break;
                    }
                } while ( !m_rear.compare_exchange_weak( cur, pos + 1 ) );
            }
        } while ( !__sync_bool_compare_and_swap( &m_statusBuffer[ locate(pos) ], NS_READABLE, NS_READPENDING ) );
    }
    fprintf( stderr, "get pos: %zd, front: %ld, rear: %ld\n", pos, m_front.load(), m_rear.load() );

    // update value
    auto ret = m_circularBuffer[ locate(pos) ];
    m_statusBuffer[ locate(pos) ]     = NS_WRITEABLE;

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
    return ( m_rear.load() >= m_front.load() );
}

template<class T>
bool RingBuffer<T>::full()
{
    return ( ( m_front.load() - m_rear.load() ) >= m_mask );
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

    fprintf( stderr, "optimisticBuffer: %ld\n", m_mask );
}

template<class T>
bool RingBuffer<T>::waitForFull()
{
    ::std::this_thread::yield();
    //std::this_thread::sleep_for(::std::chrono::milliseconds(500));

    return true;
}

template<class T>
bool RingBuffer<T>::waitForEmpty()
{
    ::std::this_thread::yield();
    //std::this_thread::sleep_for(::std::chrono::milliseconds(500));

    return true;
}

template<class T>
size_t RingBuffer<T>::locate( ::std::size_t cursor )
{
    return ( cursor & ( ~m_mask ) );
}

int main(int argc, char const *argv[])
{
    static RingBuffer<int> buffer( 100 );

    ::std::vector<::std::thread> arrWriteThread;
    for ( int index = 0; index < 1; ++index ) {
        arrWriteThread.emplace_back( [] ( int start ) {
                while ( 1 )
                {
                    if ( !buffer.full() ) {
                        buffer.put( start++ );
                        fprintf( stderr, "=== thread %zx === put: %d, size: %zd\n", ::std::hash<std::thread::id>{}( ::std::this_thread::get_id() ), start, buffer.size() );
                    }
                    std::this_thread::sleep_for(::std::chrono::milliseconds(500));
                }
            }, index * 100 ) ;
    }

    ::std::vector<::std::thread> arrReadThread;
    for ( int index = 0; index < 10; ++index ) {
        arrReadThread.emplace_back( [] {
                while ( 1 )
                {
                    if ( !buffer.empty() ) {
                        fprintf( stderr, "get: %d\n", buffer.get() );
                    }
                    std::this_thread::sleep_for(::std::chrono::milliseconds(500));
                }
            }) ;
    }

    for ( auto& t : arrWriteThread ) {
        t.join();
    }

    for ( auto& t : arrReadThread ) {
        t.join();
    }

    return 0;
}

