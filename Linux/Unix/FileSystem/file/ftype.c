#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int ftype(const char *fname)
{
    struct stat stateres;
    if(stat(fname, &stateres) < 0)
    {
        perror("stat()");
        exit(1);
    }
    // dcb-lsp
    if(S_ISREG(stateres.st_mode))
        return '-';
    else if(S_ISDIR(stateres.st_mode))
        return 'd';
    else if(S_ISCHR(stateres.st_mode))
        return 'c';
    else if(S_ISBLK(stateres.st_mode))
        return 'b';
    else if(S_ISLNK(stateres.st_mode))
        return 'l';
    else if(S_ISSOCK(stateres.st_mode))
        return 's';
    else if(S_ISFIFO(stateres.st_mode))
        return 'p';
    else
        return '?';
}

static int ftype2(const char *fname)
{
    struct stat statres;
    if(stat(fname, &statres))
    {
        perror("stat()");
        exit(1);
    }
    switch(statres.st_mode & S_IFMT)
    {
        case S_IFREG: return '-';
        case S_IFDIR: return 'd';
        case S_IFCHR: return 'c';
        case S_IFBLK: return 'b';
        case S_IFLNK: return 'l';
        case S_IFSOCK: return 's';
        case S_IFIFO: return 'p';
        default: return '?';
    }
}



int main(int argc, char **argv)
{
    if(argc < 2)
    {
        fprintf(stdout, "Bad Usage ... \n");
        exit(0);
    }

    fprintf(stdout, "%c\n", ftype(argv[1]));
    fprintf(stdout, "%c\n", ftype2(argv[1]));
}
