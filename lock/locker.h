#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <semaphore.h>
#include <pthread.h>

class sem
{
private:
    sem_t m_sem;
public:
    sem();
    sem(int num);
    ~sem();
    bool wait();
    bool post();
};

class cond
{
private:
    pthread_cond_t m_cond;
public:
    cond();
    ~cond();
    bool wait(pthread_mutex_t * mutex);
    bool signal();
};

class locker
{
private:
    pthread_mutex_t m_mutex;
public:
    locker();
    ~locker();

    pthread_mutex_t * get();
    bool lock();
    bool unlock();
};

sem::sem()
{
    if(sem_init(&m_sem, 0, 0) != 0)
    {
        throw std::exception();
    }
}

sem::sem(int num)
{
    if(sem_init(&m_sem, 0, num) != 0)
    {
        throw std::exception();
    }
}




sem::~sem()
{
    sem_destroy(&m_sem);
}

bool sem::wait()
{
    return sem_wait(&m_sem);
}

bool sem::post()
{
    return sem_post(&m_sem);
}



cond::cond()
{
    if (pthread_cond_init(&m_cond, NULL) != 0)
    {
        throw std::exception();
    }
    
}

cond::~cond()
{
    pthread_cond_destroy(&m_cond);
}

bool cond::wait(pthread_mutex_t * mutex)
{
    int ret = 0;
    //pthread_mutex_lock(&mutex);
    ret = pthread_cond_wait(&m_cond, mutex);
    //pthread_mutex_unlock(&mutex);
    return ret == 0;
}

bool cond::signal()
{
    return pthread_cond_signal(&m_cond);
}


locker::locker()
{
    if(pthread_mutex_init(&m_mutex, NULL) != 0)
    {
        throw std::exception();
    }
}

locker::~locker()
{
    pthread_mutex_destroy(&m_mutex);
}

pthread_mutex_t* locker::get()
{
    return &m_mutex;
}

bool locker::lock()
{
    return pthread_mutex_lock(&m_mutex) == 0;
}

bool locker::unlock()
{
    return pthread_mutex_unlock(&m_mutex) == 0;
}

#endif