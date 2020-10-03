#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        fprintf(stdout, "Bad Usage...\n");
        exit(1);
    }
    DIR *dp = opendir(argv[1]);
    
    struct dirent *dirp;
    while((dirp = readdir(dp)) != NULL)
        fprintf(stdout, "%s\n", dirp->d_name);
    closedir(dp);
    exit(0);
}
