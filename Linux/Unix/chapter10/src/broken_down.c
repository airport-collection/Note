#include "../../lib/tlpi_hdr.h"

#include <time.h>

void printTM(const struct tm*);

int main()
{
    const time_t sec_now = time(NULL);
    struct tm* utc = gmtime(&sec_now);
    printf("UTC Time:\n");
    printTM(utc);

    struct tm* lt = localtime(&sec_now);
    printf("LOCAL Time\n");
    printTM(lt);
    return 0;
}



void printTM(const struct tm* tm)
{
    printf("%d-%02d-%02d %02d:%02d:%02d\n", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
    tm->tm_hour, tm->tm_min, tm->tm_sec);
}