#include "../../lib/tlpi_hdr.h"
#include <fcntl.h>

#define _GNU_SOURCE

#include <unistd.h>



void retrive_ids();


int main()
{
    int result = setuid(1001);
    printf("result= %d\n", result);
    retrive_ids();
    printf("--------------------\n");
    int fd = open("/home/harrisonlee/temp/01.txt", O_RDWR);
    printf("%d\n", fd);
    printf("--------------------\n");
    retrive_ids();
    return 0;
}

void retrive_ids()
{
    uid_t real_uid;
    gid_t real_gid;
    uid_t effective_uid;
    gid_t effective_gid;
    uid_t saved_uid;
    gid_t saved_gid;
    getresuid(&real_uid, &effective_uid, &saved_uid);
    getresgid(&real_gid, &effective_gid, &saved_gid);

    printf("user ids: %d\t%d\t%d\n", real_uid, effective_uid, saved_uid);
    printf("group ids: %d\t%d\t%d\n", real_gid, effective_gid, saved_gid);
}