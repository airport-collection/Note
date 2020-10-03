#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <string.h>

extern const char *const sys_siglist[];

int main(int argc, char **argv) 
{
    printf("SIGBUS : %s\n", strsignal(SIGBUS));
    
    const char *buf = "SIGINT ";
    psignal(SIGINT, buf);
    exit(0);
}
