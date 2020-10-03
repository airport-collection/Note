#include "../../lib/tlpi_hdr.h"

#define __USE_XOPEN
#define _GNU_SOURCE
// #define _XOPEN_SOURCE //This macro cannot restrict these warning. but the above two work!
#include <time.h>

void printTM(const struct tm*);

int main()
{
    char* format = "%Y-%m-%d %X";
    const char* date_str = "1998-05-28 20:50:27";
    struct tm birthday_tm;
    char* next = strptime(date_str, format, &birthday_tm);
    if(next == NULL)
        printf("error occured when call strptime!");
    else
        printTM(&birthday_tm);
    return 0;
}

void printTM(const struct tm* tm)
{
    printf("%d-%02d-%02d %02d:%02d:%02d\n", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
    tm->tm_hour, tm->tm_min, tm->tm_sec);
}