#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 1024

// 采用write和read来实现文件复制
int main(int argc, char** argv)
{
    if(argc < 3)
    {
        fprintf(stderr, "Bad Usage...\n");
        exit(1);
    }
    int sfd = open(argv[1], O_RDONLY);
    int dfd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    
    char buf[BUFSIZE];
    int len, ret, pos;
    while((len = read(sfd, buf, BUFSIZE)) > 0)
    {
        // 其实这里是有问题的，可能写入的ret字节数小于读取的len字节数，所以需要重构一下
        pos = 0;
        while(len > 0)
        {
            fprintf(stdout, "pos = %d, size = %d\n", pos, len);
            ret = write(dfd, buf + pos, len);
            if(ret < 0)
            {
                perror("write()");
                exit(1);
            } 
            pos += ret;
            len -= ret;
        }
    }

    close(sfd);
    close(dfd);
    exit(0);
}
