#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <string.h>

static int error_handle(const char *epath, int errorno)
{
    fprintf(stdout, "glob error handle catch errors: %s\n", strerror(errorno));
    return 0;
}

int main(int argc, char **argv)
{
    glob_t globres;
    int err = glob("/etc/*.conf", 0, error_handle, &globres);
    if(err)
    {
        fprintf(stdout, "glob error code = %d\n", err);
        exit(1);
    }
    fprintf(stdout, "一共有%ld个匹配项\n", globres.gl_pathc);
    for(int i = 0; i < globres.gl_pathc; ++i)
    {
        fprintf(stdout, "%i\t%s\n", i,  globres.gl_pathv[i]);
    }
    globfree(&globres);
    exit(0);
}
