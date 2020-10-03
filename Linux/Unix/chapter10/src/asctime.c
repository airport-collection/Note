#include "../../lib/tlpi_hdr.h"

#include <time.h>

int main()
{
    time_t now_sec = time(NULL);
    struct tm* now_utc_tm = gmtime(&now_sec);
    char* str_utc_date_now = asctime(now_utc_tm);
    printf("utc time: %s\n", str_utc_date_now);

    struct tm* now_local_tm = localtime(&now_sec);
    char* str_local_date_now = asctime(now_local_tm);
    printf("local time: %s\n", str_local_date_now);
    return 0;
}