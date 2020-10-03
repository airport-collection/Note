#include "../../lib/tlpi_hdr.h"

#include <time.h>

int main()
{
    time_t seconds;
    time_t secs_tmp;
    secs_tmp = time(&seconds);
    if(secs_tmp < 0)
        printf("error occured when get seconds!");
    else
    {
        printf("seconds: %ld \n", seconds);
    }
    sleep(2);
    seconds = time(NULL);
    if(seconds < 0)
        printf("error occured when get senconds!");
    else
    {
        printf("seconds: %ld \n", seconds);
    }
    
    return 0;
}