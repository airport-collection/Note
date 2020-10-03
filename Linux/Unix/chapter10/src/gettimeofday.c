#include "../../lib/tlpi_hdr.h"

#include <sys/time.h>

typedef struct timeval timeval;
typedef struct timezone timezone;

int main()
{
    timeval tv;
    // timezone tz; // historical artifact
    int r = gettimeofday(&tv, NULL);
    if(r < 0)
        printf("error occured when get time of day!");
    else
    {
        printf("timeval:\n");
        printf("seconds: %ld, millseconds: %ld\n", tv.tv_sec, tv.tv_usec);
    }
    return 0;
}

