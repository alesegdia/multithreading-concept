// g++ -o multithreading main.cpp -lpthread --std=c++11 -g

#include "concurrency/semaphore.h"
#include "concurrency/doublebuffer.h"
#include "concurrency/paralleltaskprocessor.h"

static constexpr size_t NUM_ENTITIES = 1024;

struct Entity
{
    uint32_t data = 0;
    uint8_t extra[8];
};

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
    for(int i=0;i<1000;i++)execute_rendering();
    printf("done rendering\n");
}

void worker_update()
{
    for(int i=0;i<1000;i++)execute_update();
    printf("done update\n");
}

int main()
{
    printf("APP START ===============\n");

    ParallelTaskProcessor task_processor;
    task_processor.registerTask(worker_rendering);
    task_processor.registerTask(worker_update);
    task_processor.launchThreads();

    while(true)
    {
        task_processor.step();
        s_doubleBuffer.sync();
        printf("done all\n");
    }

    return 0;
}
