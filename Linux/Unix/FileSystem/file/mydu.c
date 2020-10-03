#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>
#include <string.h>

#define MAX_LENGTH 1024

static int path_noloop(const char * path)
{
    char *pos = strrchr(path, '/');
    if(pos == NULL)
    {
        perror("strrchr");
        exit(1);
    }
    if(strcmp(pos + 1, ".") == 0 || strcmp(pos + 1, "..") == 0)
        return 0;
    return 1;
}

static int64_t mydu(const char* path)
{
    static struct stat statres;
    if(lstat(path, &statres) < 0)
    {
        perror("lstat"); 
        exit(1);
    }
   
    if(!S_ISDIR(statres.st_mode))
        return statres.st_blocks;

    static char next[MAX_LENGTH]; 
    glob_t globres;

    strncpy(next, path, MAX_LENGTH);
    strncat(next, "/.*",  MAX_LENGTH - 10); //这里减去10完全是为了消去gcc的警告
    if(glob(next, 0, NULL, &globres))
    {
        fprintf(stdout, "glob error\n");
        exit(1);
    }

    strncpy(next, path, MAX_LENGTH);
    strncat(next, "/*", MAX_LENGTH - 10);
    if(glob(next, GLOB_APPEND, NULL, &globres))
    {
        fprintf(stdout, "glob error \n");
        exit(1);
    }

    int64_t sum;
    sum = statres.st_blocks;
    for(int i = 0; i < globres.gl_pathc; ++i)
    {
        if(path_noloop(globres.gl_pathv[i]))
            sum += mydu(globres.gl_pathv[i]);
    }
    globfree(&globres);
    return sum;
}


int main(int argc, char **argv)
{
    if(argc < 2)
    {
        fprintf(stdout, "Bad Usage...\n");
        exit(1);
    }
    fprintf(stdout, "%ld\n", mydu(argv[1]) / 2); 
    exit(0);
}
