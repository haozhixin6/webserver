#include <iostream>
#include "../lock/locker.h"
#include <unistd.h>
using namespace std;

sem * sem1;
sem * sem2;

cond * cond1;
cond * cond2;
locker * locker;

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
        locker->lock();
        cond1->wait(locker->get());
        cout << 1 << endl;
        sleep(1);
        cond2->signal();
        locker->unlock();
    }
    
}

void* test4(void*)
{
    while (1)
    {
        locker->lock();
        cond2->wait(locker->get());
        cout << 2 << endl;
        sleep(1);
        cond1->signal();
        locker->unlock();
    }
    
}

int main()
{
    sem1 = new sem();
    sem2 = new sem(1);
    // pthread_t t_1, t_2;
    // pthread_create(&t_1, NULL, test1, (void*)NULL);
    // pthread_create(&t_2, NULL, test2, (void*)NULL);
    // pthread_join(t_2, NULL);

    pthread_t t_3, t_4;
    pthread_create(&t_3, NULL, test1, (void*)NULL);
    pthread_create(&t_4, NULL, test2, (void*)NULL);
    pthread_join(t_4, NULL);
    return 0;
}