#include "../../lib/tlpi_hdr.h"
#include <grp.h>

typedef struct group group;

void print_group(group*);

int main(int argc, char* argv[])
{
    if(argc < 2)
        return 0;
    group* p = getgrnam(argv[1]);
    print_group(p);
    return 0;
}

void print_group(group* p)
{
    if(p == NULL)
        return;
    printf("group name: %s\n", p->gr_name);
    printf("password: %s\n", p->gr_passwd);
    printf("group id: %d\n", p->gr_gid);

    printf("users: ");
    char** members = p->gr_mem;
    while(*members != NULL)
    {
        printf("%s\t", *members);
        ++members;
    }
    printf("\n");
}