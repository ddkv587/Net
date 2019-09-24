#include <vector>

template<class T>
class RingBuffer final
{
    using CircleBuffer = ::std::vector<T>;

    public:
        T&      read();
        void    write( const T& );

    private:
        RingBuffer( RingBuffer& ) = delete;
        RingBuffer( const RingBuffer& ) = delete;
        RingBuffer* operator=( RingBuffer& ) = delete;

    private:
        CircleBuffer    m_bufferData;
        CircleBuffer    m_bufferAvailable;
};