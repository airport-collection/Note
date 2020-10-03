#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

static volatile int loop = 1;

static void alarm_handler(int s)
{
    loop = 0;
}

int main(int argc, char **argv)
{
    int64_t cnt = 0;
    
    alarm(5);
    signal(SIGALRM, alarm_handler);

    while(loop)
        ++cnt;
    printf("%ld\n", cnt);
    exit(0);
}
