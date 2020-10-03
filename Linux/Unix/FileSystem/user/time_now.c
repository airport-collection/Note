#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define FMT_SIZE 1024
#define BUF_SIZE 1024

int main(int argc, char **argv)
{

    char c;
    char fmtstr[FMT_SIZE];
    char *path = NULL;

    while(1)
    {
        // 在需要加传参数的后面加上冒号
        c = getopt(argc, argv, "-y:mdH:MS");
        if(c < 0)
            break;
        switch(c)
        {
            case 1:
                path = argv[optind - 1];
                break;
            case 'y':
                if(strcmp(optarg, "2") == 0)
                    strncat(fmtstr, "%y ", FMT_SIZE-10);
                else if(strcmp(optarg, "4") == 0)
                    strncat(fmtstr, "%Y ", FMT_SIZE-10);
                else
                {
                    fprintf(stderr, "the option y has now value of %s\n", optarg);
                    exit(1);
                }
                break;
            case 'm':
                strncat(fmtstr, "%m ", FMT_SIZE-10);
                break;
            case 'd':
                strncat(fmtstr, "%d ", FMT_SIZE-10);
                break;
            case 'H':
                if(strcmp(optarg, "12") == 0)
                    strncat(fmtstr, "%I(%p) ", FMT_SIZE-10);
                else if(strcmp(optarg, "24") == 0)
                    strncat(fmtstr, "%H ", FMT_SIZE-10);
                else
                {
                    fprintf(stderr, "the option has no value of %s\n", optarg);
                    exit(1);
                }
                break;
            case 'M':
                strncat(fmtstr, "%M ", FMT_SIZE-10);
                break;
            case 'S':
                strncat(fmtstr, "%S ", FMT_SIZE-10);
                break;
            default:
                {
                    fprintf(stderr, "program %s has no option of %c", argv[0], c);        
                    exit(1);
                }
        }
    }
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    char buf[BUF_SIZE];
    strftime(buf, BUF_SIZE, fmtstr, tm);
    if(path == NULL)
        fprintf(stdout, "%s\n", buf); 
    else
    {
        FILE *fp = fopen(path, "w");
        if(fp == NULL)
        {
            perror("(fopen)");
            exit(1);
        }
        fprintf(fp, "%s\n", buf);
        fclose(fp);
    }
    exit(0);
}
