#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <list>
#include <exception>
#include "../lock/locker.h"

template <typename T>
class threadpool
{
private:
    //线程数量
    int m_thread_number;

    //请求队列大小
    int m_max_requests;

    //线程池数组
    pthread_t * m_threads;

    //请求队列
    std::list<T *> m_workqueue;

    locker m_queuelocker;

    //是否有任务需要处理
    sem m_queuestat;

    //是否结束线程
    bool m_stop;

private:
    static void * worker(void * arg);

    void run();

public:
    threadpool(int thread_number = 8, int max_requests = 10000);
    ~threadpool();

    bool append(T *request);
};

template <typename T>
threadpool<T>::threadpool(int thread_number, int max_requests) : m_thread_number(thread_number), m_max_requests(max_requests), m_stop(false), m_threads(NULL)
{
    if(thread_number <= 0 || max_requests <= 0)
    {
        throw std::exception();
    }

    m_threads = new pthread_t[thread_number];

    for(int i = 0; i < thread_number; i++)
    {
        if(pthread_create(m_threads + i, NULL, worker, this) != 0)
        {
            delete[] m_threads;
            throw std::exception();
        }

        if(pthread_detach(m_threads + i))
        {
            delete[] m_threads;
            throw std::exception();
        }
    }
    
}

template <typename T>
threadpool<T>::~threadpool()
{
    delete[] m_threads;
}

template <typename T>
bool threadpool<T>::append(T * request)
{
    m_queuelocker.lock();
    if(m_workqueue.size() >= m_max_requests)
    {
        m_queuelocker.unlock();
        return false;
    }

    m_workqueue.push_back(request);
    m_queuelocker.unlock();

    m_queuestat.post();
    return true;
}

template <typename T>
void * threadpool<T>::worker(void * arg)
{
    threadpool * pool = (threadpool*)arg;
    pool->run();
    return pool;
}

template <typename T>
void threadpool<T>::run()
{
    while (true)
    {
        m_queuestat.wait();

        m_queuelocker.lock();
        if(m_workqueue.empty)
        {
            m_queuelocker.unlock();
            continue;
        }

        T * request = m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();
        if(!request)
        {
            continue;
        }
        
        request->process();
    }
}

#endif