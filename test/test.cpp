#include <iostream>
#include "../lock/locker.h"
#include "../threadpool/threadpool.h"
#include <unistd.h>
using namespace std;

sem * sem1;
sem * sem2;

cond * cond1;
locker * g_locker;

void* test1(void*)
{
    while (1)
    {
        sem2->wait();
        cout << 1 << endl;
        sleep(1);
        sem1->post();
    }
    
}

void* test2(void*)
{
    while (1)
    {
        sem1->wait();
        cout << 2 << endl;
        sleep(1);
        sem2->post();
    }
    
}

void* test3(void*)
{
    while (1)
    {
        g_locker->lock();
        cond1->wait(g_locker->get());
        cout << 1 << endl;
        sleep(1);
        g_locker->unlock();
    }
    
}

void* test4(void*)
{
    while (1)
    {
        g_locker->lock();
        cout << 2 << endl;
        sleep(1);
        cond1->signal();
        g_locker->unlock();
    }
    
}

class test
{
private:
    int m_num;
public:
    test(int num = 0) : m_num(num){};
    ~test();
    void process()
    {
        cout << m_num << " ";
    }
};



int main()
{
    //test 1
    // sem1 = new sem();
    // sem2 = new sem(1);
    // pthread_t t_1, t_2;
    // pthread_create(&t_1, NULL, test1, (void*)NULL);
    // pthread_create(&t_2, NULL, test2, (void*)NULL);
    // pthread_join(t_2, NULL);
    // delete sem1;
    // delete sem2;

    //test 2
    // cond1 = new cond();
    // g_locker = new locker();
    // pthread_t t_3, t_4;
    // pthread_create(&t_3, NULL, test1, (void*)NULL);
    // pthread_create(&t_4, NULL, test2, (void*)NULL);
    // pthread_join(t_4, NULL);
    // delete cond1;
    // delete g_locker;

    //test 3
    test * test1 = new test(1);
    test * test2 = new test(2);

    threadpool<test> * threadpooltest = new threadpool<test>();

    while (1)
    {
        threadpooltest->append(test1);
        threadpooltest->append(test2);
    }

    return 0;
}