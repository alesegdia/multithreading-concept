#pragma once

#include <cstring>
#include <cassert>

template <typename T, int N>
class DoubleBuffer
{
public:
    const T& read(size_t index)
    {
        assert(index < N);
        return m_readBuffer[index];
    }

    T& write(size_t index)
    {
        assert(index < N);
        return m_writeBuffer[index];
    }

    void sync()
    {
        memcpy((void*)m_readBuffer, (void*)m_writeBuffer, sizeof(T) * N);
    }

private:
    T m_readBuffer[N];
    T m_writeBuffer[N];

};

