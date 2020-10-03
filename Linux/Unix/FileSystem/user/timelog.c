#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define FILE_PATH "/tmp/out"
#define BUFFER_SIZE 1024

int main(int argc, char **argv) 
{
    FILE *fp = fopen(FILE_PATH, "a+");
    int cnt = 0;
    char buf[BUFFER_SIZE];
    while(fgets(buf, BUFFER_SIZE, fp) != NULL)
        ++cnt;
   time_t stamp;
   struct tm *tm;
    while(1)
    {
        time(&stamp);
        tm = localtime(&stamp);
        fprintf(fp, "%10d %-4d-%-2d-%-2d %-2d:%-2d:%-2d\n", ++cnt, tm->tm_year + 1900, tm->tm_mon + 1,
                tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
        fflush(fp);
        sleep(1);
    }
    exit(0);
}
