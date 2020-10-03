#include "../../lib/tlpi_hdr.h"

#include <time.h>

int main()
{
    const time_t sec_now = time(NULL);
    char *date_now = ctime(&sec_now);
    printf("%s\n", date_now);
    sleep(2);
    time_t sec_now2 = sec_now + 3600*24*30;
    ctime(&sec_now2);
    printf("%s\n", date_now);

    int i = 0, j = 0;
    while(*date_now != '\0')
    {
        if(*date_now == ' ')
            ++i;
        else
        {
            ++j;
            printf("-----%c-----%d\n", *date_now, j);
        }
        ++date_now;
    }
    printf("%d %d\n", i, j);
    return 0;
}