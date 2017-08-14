#pragma once

#include <mutex>
#include <condition_variable>

class Semaphore {
public:
    Semaphore (int count_ = 0)
        : m_count(count_) {}

    inline void notify()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_count++;
        m_cond.notify_one();
    }

    inline void wait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        while(m_count == 0){
            m_cond.wait(lock);
        }
        m_count--;
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_cond;
    int m_count;

};


