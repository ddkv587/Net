#include <memory>
#include <string>
#include <thread>
#include <cstdio>

using namespace std;

class A
{
public:
    A() {
        printf( "A construct\n" );
    }

    virtual ~A() {
        printf( "A destroy\n" );
    }
};

int main()
{
    // unique_ptr<int> work;

    // thread t1([&] {
    //     while (true) {
    //         const unique_ptr<int> localWork = move(work);
    //         if (localWork)
    //             printf("thread1: %d\n", *localWork);
    //         this_thread::yield();
    //     }
    // });

    // thread t2([&] {
    //     while (true) {
    //         const unique_ptr<int> localWork = move(work);
    //         if (localWork)
    //             printf("thread2: %d\n", *localWork);
    //         this_thread::yield();
    //     }
    // });

    // for (int i = 0; ; i++) {
    //     work.reset(new int(i));

    //     while (work)
    //         this_thread::yield();
    // }
    A* a = new A();
    unique_ptr<A> work1( a );
    unique_ptr<A> work2( a );

    // work1.reset();
    // work2.reset();

    return 0;
}