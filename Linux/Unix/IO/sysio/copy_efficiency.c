#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

// 测试缓冲区大小以及性能的问题
int main(int argc, char** argv)
{
    if(argc < 3)
    {
        fprintf(stderr, "Bad Usage...\n");
        exit(1);
    }
    int BUFSIZE = 128;
    while(1)
    {
        time_t begin = time(NULL);
        int sfd = open(argv[1], O_RDONLY);
        int dfd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
        
        fprintf(stdout, "======================\n");
        fprintf(stdout, "BUFSIZE: %d\n", BUFSIZE);
        char *buf = malloc(sizeof(char) * BUFSIZE);
        int len, ret, pos;
        while((len = read(sfd, buf, BUFSIZE)) > 0)
        {
            pos = 0;
            while(len > 0)
            {
                ret = write(dfd, buf + pos, len);
                if(ret < 0)
                {
                    perror("write()");
                    break;
                } 
                pos += ret;
                len -= ret;
            }
        }
        close(sfd);
        close(dfd);
    
        free(buf);
        buf = NULL;
        time_t end = time(NULL);
        fprintf(stdout, "time: %ld\n", end - begin);
        fprintf(stdout, "======================\n");
        BUFSIZE *= 2;
    }
    exit(0);
}
