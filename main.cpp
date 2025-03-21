#include <iostream>
#include "./lock/locker.h"
#include <unistd.h>
using namespace std;

sem sem1, sem2;

void* test1(void*)
{
    while (1)
    {
        sem2.wait();
        cout << 1 << endl;
        sleep(10);
        sem1.post();
    }
    
}

void* test2(void*)
{
    while (1)
    {
        sem1.wait();
        cout << 2 << endl;
        sleep(10);
        sem2.post();
    }
    
}


void main()
{
    pthread_t t_1, t_2;
    pthread_create(&t_1, NULL, test1, (void*)NULL);
    pthread_create(&t_2, NULL, test2, (void*)NULL);
    pthread_join(t_2, NULL);
}
