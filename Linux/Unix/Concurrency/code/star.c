#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void int_handler(int sig)
{
    write(1, "!", 1);
}

int main(int argc, char **argv)
{
    // signal(SIGINT, SIG_IGN);
    __sighandler_t older_handler = signal(SIGINT, int_handler);
    for(int i = 0; i < 10; ++i)
    {
        write(1, "*", 1);
        sleep(1);
    }
    write(1, "\n", 1);
    exit(0);
}
