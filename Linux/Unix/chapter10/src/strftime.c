#include "../../lib/tlpi_hdr.h"

#include <time.h>

int main()
{
    // details for format string : P194 table 10-1
    const char * format = "%Y-%m-%d %X";
    const int length_of_str = 20;
    char* date_str = malloc(sizeof(char) * length_of_str);

    time_t sec_now = time(NULL);
    struct tm* tm_now = localtime(&sec_now);
    int result = strftime(date_str, length_of_str, format, tm_now);
    if(result <= 0)
        printf("error occured when call strftime!");
    else
        printf("formatted date&time: %s\n", date_str);
    return 0;
}