#include <stdio.h>
#include <stdlib.h>

// #define _POSIX_C_SOURCE 199309
#include <signal.h>
#include <unistd.h>

static void handler1(int sig)
{
    fprintf(stdout, "!\n");
}

static void handler2(int sig, siginfo_t *siginfo, void *ucontext)
{
    fprintf(stdout, "?\n");    
}

int main(int argc, char **argv)
{
    struct sigaction act, oldact;
    act.sa_handler = handler1;
    act.sa_sigaction = handler2;
    if(sigaction(SIGINT, &act, &oldact) < 0)
    {
        perror("sigaction");
        exit(1);
    }
    while(1)
        pause();    
    exit(0);
}
