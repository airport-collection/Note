#include <stdio.h>
#include <limits.h>
#include <string.h>
void test01();
void test02();
void test03();


struct test
{
    int a;
    // union tt
    // {
    //     char c;
    //     double d;
    // };
    char c;
    long long ll;
};
enum day
{
    MON = 1,
    TUS,
    WEN,
    THR,
    FRI,
    SAT,
    SUN
};
int main()
{
    enum day monday = MON;
    printf("%d\n", monday);
    return 0;
}

void test03()
{
    float pi = 3.1415926;
    printf("%010.2f\n", pi);
    printf("%.20lf\n", pi);
}

void test02()
{
    printf("============float============\n");
    float f1 = 3.14F;
    printf("%f\n", f1);
    printf("%e\n", f1);

    printf("============double============\n");
    double d1 = 3.14;
    printf("%f\n", d1);
    printf("%e\n", d1);

    printf("============long double============\n");
    long double ld1 = 3.14;
    // 注意下列要使用%Lf或%LF, L必须大写，否则输出结果异常
    printf("%lf\n", ld1);
}

void test01()
{
    printf("============char============\n");
    char a = 'a';
    printf("%c\n", a);

    printf("============short============\n");
    short b = 65535;
    short unsigned b2 = 65535;
    // unsigned short b3 = 65538;
    printf("%hd\n", b);
    // 注意这里将有符号short转为无符号int输出，先进行符号扩展，然后再无符号输出
    printf("%u\n", b);
    printf("%hu\n", b);


    printf("============int============\n");
    int c = 3;
    unsigned int c2 = INT_MAX + 1;
    printf("%d\n", c);
    printf("%hu\n", c2);
    printf("%hd\n", c + 65536);

    printf("============long============\n");
    long d = 14000000000000000000L;
    printf("%ld\n", d);
    printf("%lu\n", d);

    printf("============long long============\n");
    long long e= 14000000000000000000L;
    printf("%lld\n", e);
    printf("%llu\n", e);
}