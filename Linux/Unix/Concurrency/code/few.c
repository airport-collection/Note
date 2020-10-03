#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    puts("Parent Progress Begin!");
    
    pid_t pid = fork();
    if(pid < 0)
    {
        fprintf(stderr, "Parent Progress Fork failed!\n");
        exit(1);
    }
    
    if(pid == 0)
    {
        execl("/usr/bin/date", "这是子进程的名字", "+%s", NULL);
        perror("execl");
        exit(1);
    }
    else
        wait(NULL);
    puts("Parent Progress End!");
    exit(0);
}
