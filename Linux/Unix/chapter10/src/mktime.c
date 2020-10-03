#include "../../lib/tlpi_hdr.h"

#include <time.h>

int main()
{
    struct tm* tm_now = malloc(sizeof(struct tm));
    tm_now->tm_year = 1998 - 1900;
    tm_now->tm_mon = 5 - 1;
    tm_now->tm_mday = 28;
    tm_now->tm_hour = 20;
    tm_now->tm_min = 50;
    tm_now->tm_sec = 27;
    tm_now->tm_isdst = 0;
    time_t sec_now = mktime(tm_now);
    if(sec_now < 0)
        printf("error occured when call mktime!\n");
    else
        printf("%ld\n%ld\n", sec_now, time(NULL));
    free(tm_now);
    tm_now = NULL;
    return 0;
}