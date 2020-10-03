#include "../../lib/tlpi_hdr.h"
#include <unistd.h>

void retrive_ids();
void set_ids();

int main()
{
    set_ids();
    retrive_ids();
    return 0;
}

void set_ids()
{
    int result = seteuid(0);
    printf("result: %d\n", result);
}

void retrive_ids()
{
    uid_t real_uid = getuid();
    gid_t real_gid = getgid();
    uid_t effective_uid = geteuid();
    gid_t effective_gid = getegid();

    printf("%d\n", real_uid);
    printf("%d\n", real_gid);
    printf("%d\n", effective_uid);
    printf("%d\n", effective_gid);
}