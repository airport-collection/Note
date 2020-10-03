#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

static void *dowork(void *x)
{
    int64_t sum = 0;
    int i = 0;
    int y = *(int*)x;
    while(i <= y)
        sum += i++;
    printf("%ld\n", sum);
    pthread_exit(NULL); // 这种方式比直接return更好，可以进行一些清理工作
    // return NULL;
}

int main(int argc, char **argv)
{
    puts("Begin!");
    pthread_t tid;
    int i = 100;
    int err = pthread_create(&tid, NULL, dowork, &i);
    if(err)
    {
        fprintf(stderr, "pthread_create failed: %s\n", strerror(err));
        exit(1);
    }
    pthread_join(tid, NULL);
    puts("End!");
    exit(0);
}
