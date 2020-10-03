

# const与extern

与一般对象不同，const对象要求在定义时，要加上extern, 否则无效。

 # const与引用

* const引用可以指向非const对象，但是不能通过该引用来进行修改

* 非const引用不能指向const对象

* **当引用指向了临时空间，该引用必须是const的。**

  直接将字面量或者是不同类型的变量赋值给引用，那么编译器会创建临时空间存放值，然后让引用指向该临时空间。



# const与指针

* const int *：常量指针，指针本身不是常量，可以不用初始化
* int *const：指针常量，指针本身就是一个常量，必须初始化

* const int * const：指向常量的指针常量
* 非const指针不能指向const对象





# const与表达式

常量表达式是指在编译时就能够被确定值的表达式。

可以用关键字**constexpr**指定表达式是常量表达式。注意，赋值符右边的内容必须让编译器在编译时能够被确定，否则报错。

constexpr指针指向的内容必须是nullprt, 0或地址固定的对象。

```c
int i = 10;
int j = 11;
int main()
{
    constexpr int *p = &i;
    *p = 11; // OK!!!
    // p = &j;  Error!!!
    exit(0);
}
```

* const int *p:  常量指针
* constexpr int *p: 指针常量