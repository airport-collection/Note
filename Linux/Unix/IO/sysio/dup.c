#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


// 输出重定向及恢复
int main(int argc, char **argv)
{
    const char *file = "/tmp/out"; 
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if(fd < 0)
    {
        perror("open()");
        exit(1);
    }
    // puts("before redirection!");
    //更改1号fd时先备份stdout到其他元素中
    int stdout_tmp = dup(1);

    close(1);
    int newid = dup(fd);
    if(newid < 0)
    {
        perror("dup()");
        fprintf(stdout, "重定向失败\n");
        exit(1);
    }
    // 这句话输出到文件中
    puts("hello world!");
    /*
     * !!! 注意
     * 首先上述采用的标准IO来输出的，puts会自动加入换行符，按理说会刷新缓冲区。
     * 但是，上述语句最终也被输出到stdout中，可以推测上述语句被放入用户空间中的缓冲区后没有被刷新，最后等恢复stdout后，进程结束自动刷新缓冲区再被输出，结果就被输出到stdout中了。
     *
     * 疑问就在于明明标准输出采用的是行缓冲模式，puts又会自动加入换行符，为什么会产生这样的结果？
     */
    // fflush(stdout);
    
    // 关闭重定向的文件，腾出空间准备恢复stdout
    close(1);
    close(fd);
    // 让stdout重新恢复到1号位置
    newid = dup(stdout_tmp);
    if(newid < 0)
    {
        perror("dup()");
        fprintf(stdout, "重定向恢复失败\n");
        exit(1);
    }
    // 这句话输出到终端上
    puts("completed!");
    exit(0);
}
