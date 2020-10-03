#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

static void cleanup(void *p)
{
    char *s = (char*)p;
    puts(s);
}

static void *dowork(void *p)
{
    puts("thread begin!");

    pthread_cleanup_push(cleanup, "I");
    pthread_cleanup_push(cleanup, "Love");
    pthread_cleanup_push(cleanup, "You");

    pthread_exit(NULL);
    
    /**
     * 通常情况下，如果参数为1, 那么取下来的cleanup函数就会被执行，否则就不被执行。
     * 但是这里的函数位于pthread_exit之后，是无法被执行到的，所以所有都默认是1，即都执行。
     *
     * 但是的三个函数必须和上面的三个函数对应，否则编译都通不过，因为这两个函数是宏，如果不成对出现，会出现大括号不匹配
     */
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);

    puts("thread end!");
}

int main(int argc, char **argv)
{
    pthread_t tid;

    int err = pthread_create(&tid, NULL, dowork, NULL);
    if(err)
    {
        fprintf(stderr, "pthread_create: %s\n", strerror(err));
        exit(1);
    }
    pthread_join(tid, NULL);
    exit(0);
}
