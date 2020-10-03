#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static off_t flen(const char* fname)
{
    struct stat statres;
    int res = stat(fname, &statres);
    if(res < 0)
    {
        perror("stat()");
        exit(1);
    }
    return statres.st_size;
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        fprintf(stdout, "Bad Usage...\n");
        exit(1);
    }
    fprintf(stdout, "%ld\n", flen(argv[1])); 
    exit(0);
}
