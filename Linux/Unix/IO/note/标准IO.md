标准IO中，FILE类型（其实是一个结构体）贯穿始终，所有的操作都依赖于该类型。
gcc在未找到函数声明的时候，默认所有函数的返回值都为整型。
error.h的路径为 **/usr/include/errno.h**, 也可以参考 **/usr/include/asm-generic/errno-base.h**

* fopen();
* fclose();

linux下文件创建的权限默认值为： 0666 & ～umask (这里的值为八进制数)

* fgetc();
* fputc();
* getchar()=getc(stdin)=fgetc(stdin)
* putchar(c)=putc(c, stdout)=fputc(c, stdout)
    getc()和fgetc()通常都是一样的，但是getc是一个宏函数。    

* fgets();
* fputs();

* fread();
* fwrite();


* printf();
* scanf();
* fprintf();
* sprintf();
* snprintf();

* ftell();
* fseek();
* rewind();
上面这两个函数只能处理大小低于2G的文件
* fseeko();
* ftello();

* fflush();
一般缓冲有三种模式：
    行缓冲：满了的时候刷新，强制刷新时输出，换行时候刷新（stdout）
    全缓冲：满了的时候刷新，强制刷新时输出（默认，但终端设置除外）
    无缓冲：stderr, 立即输出,

* getline();
    gcc提供的函数，采用动态内存实现，一次性读取一行
