#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024

// 使用fgets()来实现复制文件的功能
int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "Error usage: %s <src_file> <dest_file>\n", argv[0]);
        exit(1);        
    }
    FILE *src = NULL, *dest = NULL;
    src = fopen(argv[1], "r");
    if(src == NULL)
    {
        fprintf(stderr, "source file doesn't exist!\n");
        exit(1);
    }
    dest = fopen(argv[2], "w");
    if(dest == NULL)
    {
        fclose(src);
        fprintf(stderr, "destination file doesn't exist!\n");
        exit(1);
    }
    char buf[BUFSIZE];
    while(fgets(buf, BUFSIZE, src) != NULL)
        fputs(buf, dest);
    fclose(dest);
    fclose(src);
    exit(0);
}
