// g++ -o multithreading main.cpp -lpthread --std=c++11 -g

#include "concurrency/semaphore.h"
#include "concurrency/doublebuffer.h"
#include "concurrency/paralleltaskprocessor.h"

#include <iostream>

static constexpr size_t NUM_ENTITIES = 1024;

struct Entity
{
    uint32_t data = 0;
    uint8_t extra[8];
};

static DoubleBuffer<Entity, NUM_ENTITIES> s_doubleBuffer;

class Renderer
{
public:
    void render()
    {
        for( int t = 0; t < 1000; t++ )
        {
            for( int i = 0; i < NUM_ENTITIES; i++ )
            {
                const Entity& e = s_doubleBuffer.read(i);
                Entity foo = e;
                Entity bar = foo = e;
                foo = bar = e;
            }
        }
        printf("done rendering\n");
    }
};

class Updater
{
public:
    void update()
    {
        for( int t = 0; t < 1000; t++ )
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
        std::cout << ("done update\n");
    }
};


void do_ai()
{
    int integer;
    for( int i = 0; i < 100; i++ )
    {
        float a = 12345;
        float b = 34934;
        float c = 23883;
        float x = (c + a * c - a * a * c - (c + b + a)) / (a + b * c + b);
        float y = x + 23;
        float z = x + y - 432;
        integer = z;
    }
    std::cout << ("done AI\n");
}

#include <unistd.h>
int main()
{
    printf("APP START ===============\n");

    ParallelTaskProcessor task_processor;
    Renderer renderer;
    Updater updater;

    task_processor.registerTask(&renderer, &Renderer::render);
    task_processor.registerTask(&updater, &Updater::update);
    task_processor.registerTask(do_ai);
    task_processor.launchThreads();


    while(true)
    {
        task_processor.step();
        s_doubleBuffer.sync();
        std::cout << ("done all\n\n");
        fflush(0);
    }

    return 0;
}
