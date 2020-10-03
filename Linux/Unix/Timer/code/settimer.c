#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

/*
 * 定时器
 */
static void handler1(int sig)
{
    printf("$"); 
    fflush(stdout);
    struct itimerval curr_value;
    if(getitimer(ITIMER_REAL, &curr_value) >= 0) {
        printf("next: %lds %ldus\n", curr_value.it_value.tv_sec, curr_value.it_value.tv_usec);
    }
}

int main(int argc, char **argv)
{
    struct sigaction act, oldact;
    act.sa_handler = handler1;
    if(sigaction(SIGALRM, &act, &oldact) < 0)
    {
        perror("sigaction");
        exit(1);
    }
    struct itimerval new_value, old_value;
    new_value.it_interval.tv_sec = 0;
    // 下面微秒的值如果为1e6也就是一秒的话，没有效果
    new_value.it_interval.tv_usec = 1000000 - 1;
    new_value.it_value.tv_sec = 1;
    new_value.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &new_value, &old_value);
    while(1)
        pause();
    sigaction(SIGALRM, &oldact, NULL);
    exit(0);
}
