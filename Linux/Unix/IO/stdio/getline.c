#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Error Usage...\n");
        exit(1);
    }
    char *str;
    size_t n;

    /***!!!IMPORTANT***/
    str = NULL;
    n = 0;
    FILE *file = fopen(argv[1], "r");
    if(file == NULL)
    {
        fprintf(stderr, "file open failed\n");
        exit(1);
    }
    while(1)
    {
        if(getline(&str, &n, file) < 0)
            break;
        fprintf(stdout, "%ld\n", strlen(str));
    }
    fclose(file);
    free(str); //但不推荐这样做
    exit(0);
}
