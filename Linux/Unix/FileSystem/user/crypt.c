#include <stdio.h>
#include <stdlib.h>
#include <shadow.h>
#include <crypt.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        fprintf(stderr, "Bad Usage...\n");
        exit(1);
    }
    char *input_pass = getpass("Please Input Your Password: ");
    struct spwd *shadow_line = getspnam(argv[1]);
    char *crypted_pass = crypt(input_pass, shadow_line->sp_pwdp);
    if(strcmp(shadow_line->sp_pwdp, crypted_pass) == 0)
        puts("Entry Success");
    else
        puts("Entry Failed");
    exit(0);
}
