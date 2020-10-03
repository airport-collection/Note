#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

static void handler(int sig)
{
    putc('!', stdout);
    fflush(stdout);
}

int main(int argc, char **argv)
{
    if(signal(SIGINT, handler) == SIG_ERR && errno > 0)
    {
        perror("signal");
        exit(1);
    }
    while(1)
    {
        putc('*', stdout);
        fflush(stdout);
        sleep(1);
    }
    exit(0);
}
