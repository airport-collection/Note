#include <stdio.h>
#include <stdlib.h>

// count the number of charactr of a file.
int main(int argc, char** argv)
{
    if(argc < 2)
    {   
        fprintf(stderr, "Error Usage...\n");
        exit(1);
    }
    FILE *file = fopen(argv[1], "r");
    int count = 0;
    while(fgetc(file) != EOF)
        ++count;
    printf("the file %s totally has %d characters\n", argv[1], count);
    exit(0);
}
