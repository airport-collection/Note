/**
 * 四个线程在终端上面轮流分别打印a, b, c, d.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define THR_NUM 4

static pthread_mutex_t mutexs[THR_NUM];

static void *dowork(void *p)
{
    int i = (int)p;
    while(1)
    {
        pthread_mutex_lock(mutexs + i);
        putchar('a' + i);
        pthread_mutex_unlock(mutexs + ((i + 1) % THR_NUM));
    }
    pthread_exit(NULL);
}
int main()
{
    pthread_t tids[THR_NUM];
    for(int i = 0; i < THR_NUM; ++i)
    {
        pthread_mutex_init(mutexs + i, NULL);
        pthread_mutex_lock(mutexs + i);
        pthread_create(tids + i, NULL, dowork, (void *)i);
    }
    pthread_mutex_unlock(mutexs);

    alarm(5); 
    for(int i = 0; i < THR_NUM; ++i)
    {
        pthread_join(tids[i], NULL);
        pthread_mutex_destroy(mutexs + i);
    }
    exit(0);
}
