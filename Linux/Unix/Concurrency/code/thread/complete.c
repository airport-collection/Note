#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUM 20
#define PATH "/tmp/out"
#define BUF_SIZE 1024

static pthread_mutex_t mutex;

static void *dowork(void *p)
{
    FILE *fp = fopen(PATH, "r+");
    char buf[BUF_SIZE];

    pthread_mutex_lock(&mutex);
    fgets(buf, BUF_SIZE, fp);
    sleep(1);
    int i = atoi(buf) + 1;
    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "%d\n", i);
    fclose(fp);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    pthread_mutex_init(&mutex, NULL);
    pthread_t tids[THREAD_NUM];
    for(int i = 0; i < THREAD_NUM; ++i)
    {
        int err = pthread_create(tids + i, NULL, dowork, NULL);
        if(err)
        {
            perror("pthread_create");
            exit(1);
        }
    }
    for(int i = 0; i < THREAD_NUM; ++i)
        pthread_join(*(tids + i), NULL);
    
    pthread_mutex_destroy(&mutex);
    exit(0);
}
