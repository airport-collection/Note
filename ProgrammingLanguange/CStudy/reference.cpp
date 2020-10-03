#include<iostream>
using namespace std;

int main()
{
    char* p = "hello";
    char* &q = p;

    printf("%p\n", p);
    printf("%p\n", q);
    return 0;
}