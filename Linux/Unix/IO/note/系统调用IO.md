# 系统调用IO
与标准IO的FILE类型一样，系统调用IO中使用文件描述符来贯穿始终，很多操作都要使用到文件描述符。

## 文件描述符号
简称fd, 是一个整型数，同样也是一个数组下标，每次open一个文件，返回的fd是所有可能下标中最小的那个。
为了更好地理解fd, 需要了解下文件描述符和打开的文件之间的关系。
关于文件描述符，内核维护了三个数据结构如下：
* 每个进程自己的文件描述符表（上面所说的数组下标中的数组就是指的该数组）
* 系统范围的打开文件的列表（open file table or: oepn file descriptions）
* 文件系统i-node表
关于三者的关系，在TLPI的95页有图示。

## 文件IO（系统调用IO）相关操作函数
open, close, read, write, lseek

!!! 不要将文件IO和标准IO混用！！！

文件IO和标准IO在缓冲区方面的差异，参考TLPI的第13章。
