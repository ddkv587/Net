#include <memory>
#include <atomic>
#include <thread>
#include <vector>
#include <mutex>
#include <tuple>

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

        bool                            put( const T& );
        ::std::tuple<T, bool>           get();
        
        void                            reset();
        void                            resize( ::std::size_t size );

        ::std::size_t                   size()  const;
        ::std::size_t                   capacity()  const;

    protected:
        virtual void                    optimisticBuffer( ::std::size_t size );

        virtual bool                    waitForFull();
        virtual bool                    waitForEmpty();

    private:
        inline size_t                   locate( ::std::size_t cursor );

    private:
        ::std::atomic_size_t                m_front;
        ::std::atomic_size_t                m_rear;
        ::std::size_t                       m_mask;
        ::std::mutex                        m_mutex;

        ::std::unique_ptr<T[]>              m_circularBuffer;
        ::std::unique_ptr<ENodeStatus[]>    m_statusBuffer;
};

template<class T>
RingBuffer<T>::RingBuffer( ::std::size_t size )
    : m_front( 0 )
    , m_rear( 0 )
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
    ::std::size_t pos = m_front.fetch_add(1);
    while( !__sync_bool_compare_and_swap( &m_statusBuffer[ locate(pos) ], NS_WRITEABLE, NS_WRITEPENDING ) ) {
        if ( m_statusBuffer[ locate(pos) ] == NS_READPENDING || m_statusBuffer[ locate(pos) ] == NS_READABLE ) {
            // wait for read complete
            continue;
        }

        pos = m_front.fetch_add(1);
    }
    fprintf( stderr, "put pos: %ld, front: %ld, rear: %ld\n", pos, m_front.load(), m_rear.load() );

    // update data
    m_circularBuffer[ locate(pos) ]   = node;
    m_statusBuffer[ locate(pos) ]     = NS_READABLE;

    return true;
}

template<class T>
::std::tuple<T, bool> RingBuffer<T>::get()
{   
    // local the avaliable place
    ::std::size_t pos = m_rear.fetch_add(1);
    while( !__sync_bool_compare_and_swap( &m_statusBuffer[ locate(pos) ], NS_READABLE, NS_READPENDING ) ) {
        if ( m_statusBuffer[ locate(pos) ] == NS_WRITEPENDING || m_statusBuffer[ locate(pos) ] == NS_WRITEABLE ) {
            // wait for write complete
            continue;
        }

        pos = m_rear.fetch_add(1);
    }
    fprintf( stderr, "get pos: %zd, front: %ld, rear: %ld\n", pos, m_front.load(), m_rear.load() );

    // update value
    auto ret = m_circularBuffer[ locate(pos) ];
    m_statusBuffer[ locate(pos) ]   = NS_WRITEABLE;

    return ::std::make_tuple( ret, true );
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
    for ( int index = 0; index < 10; ++index ) {
        arrWriteThread.emplace_back( [] ( int start ) {
                while ( 1 ) {
              
                        buffer.put( start++ );
                        fprintf( stderr, "=== thread %zx === put: %d, size: %zd\n", ::std::hash<std::thread::id>{}( ::std::this_thread::get_id() ), start, buffer.size() );
                
                    std::this_thread::sleep_for(::std::chrono::milliseconds(500));
                }
            }, index * 100 ) ;
    }

    ::std::vector<::std::thread> arrReadThread;
    for ( int index = 0; index < 1; ++index ) {
        arrReadThread.emplace_back( [] {
                while ( 1 ) {
           
                        auto value = buffer.get();
                        if ( std::get<1>( value ) ) {
                            fprintf( stderr, "get: %d\n", std::get<0>( value ) );
                        } else {
                            fprintf( stderr, "get value failed\n");
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

