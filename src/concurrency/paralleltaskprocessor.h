#pragma once

#include <functional>
#include <vector>
#include <thread>

#include "semaphore.h"

class ParallelTaskProcessor
{
public:

    static constexpr size_t MaxThreads = 10;

    void registerTask(std::function<void(void)> task)
    {
        createTaskWorker(task);
    }

    template <typename T>
    void registerTask(T* object, void (T::*method)() )
    {
        createTaskWorker(std::bind(method, object));
    }

    void launchThreads()
    {
        for( int i = 0; i < m_tasks.size(); i++ )
        {
            auto task = m_tasks[i];
            m_threads.push_back(new std::thread(task));
        }
    }

    void step()
    {
        notifyStartThreads();
        waitFinishThreads();
    }

private:
    void waitFinishThreads()
    {
        for( int i = 0; i < m_threads.size(); i++ )
        {
            m_endSem.wait();
        }
    }

    void notifyStartThreads()
    {
        for( int i = 0; i < m_threads.size(); i++ )
        {
            m_startSem[i].notify();
        }
    }

private:
    void createTaskWorker(std::function<void(void)> worker_process)
    {
        size_t index = m_tasks.size();
        assert(index < ParallelTaskProcessor::MaxThreads);

        Semaphore& worker_semaphore = m_startSem[index];
        m_tasks.push_back([this, worker_process, &worker_semaphore]()
        {
            std::function<void(void)> worker = worker_process;
            while(true)
            {
                worker_semaphore.wait();
                worker();
                this->m_endSem.notify();
            }
        });
    }

    std::vector<std::function<void(void)>> m_tasks;
    Semaphore m_startSem[ParallelTaskProcessor::MaxThreads];
    Semaphore m_endSem;
    std::vector<std::thread*> m_threads;

};


