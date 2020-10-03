#include "../../lib/tlpi_hdr.h"
#include <pwd.h>

typedef struct passwd passwd;

void print_passwd(passwd*);

int main(int argc, char* argv[])
{
    // passwd* p = getpwnam("root");
    // print_passwd(p);
    // printf("---------------------------\n");
    // p = getpwuid(1000);
    // print_passwd(p);

    passwd* p;
    setpwent();
    while((p = getpwent()) != NULL)
    {
        print_passwd(p);
        printf("---------------------------\n");
    }
    endpwent();
    return 0;
}

passwd* get_passwd_byname(const char* name)
{
    passwd* p = getpwnam(name);
    if(p == NULL)
    {
        if(errno == 0)
            printf("The user specified by this name doesn't exist.\n");
        else
            printf("Other errors occured.\n");
    }
    return p;
}

void print_passwd(passwd* p)
{
    if(p == NULL)
        return;
    printf("username: %s\n", p->pw_name);
    printf("password: %s\n", p->pw_passwd);
    printf("user id: %d\n", p->pw_uid);
    printf("group id: %d\n", p->pw_gid);
    printf("comment: %s\n", p->pw_gecos);
    printf("home dir: %s\n", p->pw_dir);
    printf("shell: %s\n", p->pw_shell);
}