#include "../../lib/tlpi_hdr.h"
#include <unistd.h>
#include <gnu/libc-version.h>

/**
 * some ways to contrieve the version of glibc
 */

int main()
{

    printf("version: %d.%d\n", __GLIBC__, __GLIBC_MINOR__);

    const char *s = gnu_get_libc_version();
    printf("version: %s\n", s);

    printf("version: %d\n", _CS_GNU_LIBC_VERSION); // equal to __GLIBC__
    // printf("version: %d\n", confstr());
    return 0;
}