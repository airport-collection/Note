Vim中有很多关于tab的设置，很容易搞混，所以特此记录一下：
参考链接： https://zhuanlan.zhihu.com/p/51017142

tabstop
    该选项表示vim将一个tab看作多少个space
softtabstop
    该选项表示tab会被解释为tab和space的组合, 比如softtabstop设置为7, 而tabstop为4, 那么当按tab键时，会插入一个tab字符和3个space字符
expandtab
    该选项表示vim会把tab转变为space
shiftwidth
    该选项表示将tab转变为space后，具体的对应关系，比如设置该属性值为4, 那么1 tab = 4 space
