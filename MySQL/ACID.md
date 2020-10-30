# 原子性

快照

MySQL中的日志有以下几类：

* undo log
* redo log
* bin log
* relay log
* slow log
* ......

其中undo log和redo log是引擎层面的，比如在InnoDB中存在，在MyIsam中就不存在，因为其根本就不支持事务，就没必要需要这两类日志。

## Undo log

Undo  log实现了事务的原子性，在InnoDB中还用其实现多版本并发控制（MVCC）。在操作任何数据之前，首先将数据备份到一个地方（undo log），然后进行数据的修改，如果出现了错误或者用户执行了ROLLBACK，系统可以利用undo log中的备份将数据恢复到事务开始之前的状态。

undo log为逻辑日志，不是记录具体的数据，而是记录对数据进行的操作，比如insert和update。

# 一致性



# 隔离性

利用锁来实现

# 持久性：

fsync，强制刷新缓冲区

## redo log

在/var/lib/mysql/目录下存在两个文件，叫作ib_logfile0和ib_logfile1。

redo log采用循环写的策略，避免查找记录。