#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        fprintf(stdout, "Bad Usage...\n");
        exit(1);
    }
    
    struct passwd *pd = getpwnam(argv[1]);
    if(pd != NULL)
        fprintf(stdout, "the id for user %s is %d\n", argv[1], pd->pw_gid);
    else
        fprintf(stdout, "the user %s is not exist on the system\n", argv[1]);
    exit(0);
}
