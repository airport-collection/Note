#include "../../lib/tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

void read_from_terminal();
void file_descriptor();

#define MAX_READ 20+1
char buffer[MAX_READ];

extern char **environ;

int main(int argc, char*argv[])
{
    // while(environ != NULL)
    // {
    //     puts(environ);
    //     ++environ;
    // }
    char* browser = getenv("BROWSER");
    printf("%s\n", browser);
    return 0;
}

void file_descriptor()
{
    int a = open("../temp/test1.txt", O_WRONLY);
    close(a);
    int b = open("../temp/test1.txt", O_RDONLY);
    close(b);
    int c = open("../temp/test1.txt", O_RDWR);
    close(c);

    printf("a = %d, b = %d, c = %d\n", a, b, c);
}

void read_from_terminal()
{
    if(read(STDIN_FILENO, buffer, MAX_READ) == -1)
        errExit("read");
    buffer[MAX_READ-1] = '\0';
    // int offset = lseek(STDIN_FILENO, 0, SEEK_CUR);// use lseek for terminal is not permitted
    // printf("%d\n", offset);
    printf("the input data was %s \n", buffer);
}