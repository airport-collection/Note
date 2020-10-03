#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
    FILE *file;
    file = fopen("tmp", "r");
    if(file == NULL)
    {
        //printf("fopen failed! errno = %d\n", errno);
        // perror("fopen()"); // 自动将errno转为errmsg
        char* errmsg = strerror(errno);
        printf("%s\n", errmsg);
        exit(1);
    }
    else
    {
        printf("file open succeed! error = %d\n", errno);
    }
    
    exit(0);
}
