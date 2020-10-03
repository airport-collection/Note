int settimer(int which, const struct itimerval \*new_value, struct itimerval \*old_value)
其中which有三类值，一个进程只能有一个计时器，如果多次调用，会改变当前计时器的属性。
new_value和old_value都指向同一类型的结构体，该结构体内部有两个字段，分别为it_interval和it_value, 类型都是struct timeval的,包含秒和微秒两个字段。
    it_interval表示该定时器是否是周期定时器, 如果该字段的秒和微秒字段都为0的话，那么该定时器只会作用一次，否则则表明这个一个周期定时器。
    it_value表示距离下一次过期的时间。

-----------------------------------------------
