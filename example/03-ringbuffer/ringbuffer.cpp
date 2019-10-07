#include <memory>
#include <mutex>
#include <thread>
#include <chrono>

template <class T>
class RingBuffer final
{
public:
    explicit RingBuffer(::std::size_t size)
        : m_capacity(size + 1)
        , m_circularBuffer(new T[size + 1])
    {
        m_front = 0;
        m_rear = 0;
    }

    bool put(T t)
    {
        ::std::lock_guard<::std::mutex> lock(m_mutex);

        if ((m_front + 1) % m_capacity == m_rear)
        {
            // full
            return false;
        }
        else
        {
            m_circularBuffer[m_front] = t;
            m_front = (m_front + 1) % m_capacity;
        }

        return true;
    }

    T get()
    {
        ::std::lock_guard<::std::mutex> lock(m_mutex);

        if (m_rear == m_front)
        {
            // empty
            return T();
        }
        else
        {
            auto& ret = m_circularBuffer[m_rear];
            m_rear = (m_rear + 1) % m_capacity;

            return ret;
        }
    }

    void reset()
    {
        ::std::lock_guard<::std::mutex> lock(m_mutex);

        m_front = 0;
        m_rear = 0;
    }

    bool empty()
    {
        ::std::lock_guard<::std::mutex> lock(m_mutex);
        return (m_rear == m_front);
    }

    bool full()
    {
        ::std::lock_guard<::std::mutex> lock(m_mutex);

        return ((m_front + 1) % m_capacity == m_rear);
    }

    size_t capacity() const
    {
        return (m_capacity - 1);
    }

    size_t size() const
    {
        ::std::lock_guard<::std::mutex> lock(m_mutex);

        return (m_rear - m_front + m_capacity) % m_capacity;
    }

private:
    RingBuffer(RingBuffer &) = delete;
    RingBuffer(const RingBuffer &) = delete;
    RingBuffer *operator=(RingBuffer &) = delete;

private:
    ::std::size_t m_front;
    ::std::size_t m_rear;
    ::std::size_t m_capacity;
    ::std::mutex m_mutex;

    ::std::unique_ptr<T[]> m_circularBuffer;
};

int main(int argc, char const *argv[])
{
    static RingBuffer<int> buffer(10);

    ::std::thread t1(
        [] ( int start ) {
            while ( 1 )
            {
                if ( buffer.full() )
                {
                    printf("full\n");
                }
                else
                {
                    printf("put: %d\n", start);
                     buffer.put(start++);
                }
                std::this_thread::sleep_for(::std::chrono::milliseconds(1000));
            }
        }, 10);

    ::std::thread t2(
        [] {
            while ( 1 )
            {
                if (buffer.empty())
                {
                    printf("empty\n");
                }
                else
                {
                    printf("get: %d\n", buffer.get());
                }
                std::this_thread::sleep_for(::std::chrono::milliseconds(2000));
            }
        });

    t1.join();
    t2.join();

    return 0;
}
