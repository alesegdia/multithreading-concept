// g++ -o multithreading main.cpp -lpthread --std=c++11 -g

#include <thread>
#include <cassert>
#include <functional>
#include <cstring>

#include "concurrency/semaphore.h"
#include "concurrency/doublebuffer.h"

static constexpr size_t NUM_ENTITIES = 1024;

struct Entity
{
    uint32_t data = 0;
    uint8_t extra[8];
};


static Semaphore s_initSemaphore;
static Semaphore s_finishSemaphore;
static DoubleBuffer<Entity, NUM_ENTITIES> s_doubleBuffer;

void execute_rendering()
{
    for( int i = 0; i < NUM_ENTITIES; i++ )
    {

        const Entity& e = s_doubleBuffer.read(i);
        Entity foo = e;
        Entity bar = foo = e;
        foo = bar = e;
    }
}

void execute_update()
{
    int offset = std::rand();
    for( int i = 0; i < NUM_ENTITIES; i++ )
    {
        Entity& e = s_doubleBuffer.write(i);
        e.data = offset + i;
        Entity dumb = e;
        e.extra[0] = 0xFE;
        e = dumb;
    }
}

void worker_rendering()
{
    while(true)
    {
        s_initSemaphore.wait();
        for(int i=0;i<1000;i++)execute_rendering();
        printf("done rendering\n");
        s_finishSemaphore.notify();
    }
}

void worker_update()
{
    while(true)
    {
        s_initSemaphore.wait();
        for(int i=0;i<1000;i++)execute_update();
        printf("done update\n");
        s_finishSemaphore.notify();
    }
}


int main()
{
    printf("APP START ===============\n");
    std::thread thread_rendering(worker_rendering);
    std::thread thread_update(worker_update);

    while(true)
    {
        s_initSemaphore.notify();
        s_initSemaphore.notify();

        s_finishSemaphore.wait();
        s_finishSemaphore.wait();

        s_doubleBuffer.sync();
        printf("done all\n");
    }

    return 0;
}
