# 并发基础

## CAS和AQS

什么是CAS？什么又是AQS？



CAS怎么解决ABA问题？版本号，时间戳

CAS底层是如何保证原子性的？加Lock指令 https://www.hicsc.com/post/2020031059

lock指令让CPU在执行下一条指令时锁住总线，从而让CAS具有原子性。

## synchronized

在字节码层面，synchronized修饰代码块时会产生两条指令，分别为`monitorenter`和`monitorexit`，修饰方法时，并没有这两条指令，而是多了一个flag为`ACC_SYNCHRONIZED`(0x0020)。

尽管synchronized修饰的位置不一样，字节码也会不一样，但是本质上都是对对象的monitor进行处理。

```c++
IRT_ENTRY_NO_ASYNC(void, InterpreterRuntime::monitorenter(JavaThread* thread, BasicObjectLock* elem))
#ifdef ASSERT
  thread->last_frame().interpreter_frame_verify_monitor(elem);
#endif
  if (PrintBiasedLockingStatistics) {
    Atomic::inc(BiasedLocking::slow_path_entry_count_addr());
  }
  Handle h_obj(thread, elem->obj());
  assert(Universe::heap()->is_in_reserved_or_null(h_obj()),
         "must be NULL or an object");
  /**
     是否启用偏向锁
     默认开启，可以通过参数来控制
     -XX:+UseBiasedLocking
     -XX:BiasedLockingStartupDelay=0000 (之前该参数的默认值为4000，即4s,但是JDK-818042改为了0)
  */
  if (UseBiasedLocking) {
    // Retry fast entry if bias is revoked to avoid unnecessary inflation
    ObjectSynchronizer::fast_enter(h_obj, elem->lock(), true, CHECK);
  } else {
    ObjectSynchronizer::slow_enter(h_obj, elem->lock(), CHECK);
  }
  assert(Universe::heap()->is_in_reserved_or_null(elem->obj()),
         "must be NULL or an object");
#ifdef ASSERT
  thread->last_frame().interpreter_frame_verify_monitor(elem);
#endif
IRT_END
```

如果启用了偏向锁，会执行fast_enter函数：

```c++
void ObjectSynchronizer::fast_enter(Handle obj, BasicLock* lock,
                                    bool attempt_rebias, TRAPS) {
  // 再次判断是否开启偏向锁，如果未开启，直接执行下面的slow_enter
  if (UseBiasedLocking) {
    if (!SafepointSynchronize::is_at_safepoint()) { // 如果在安全点
      // 尝试获取偏向锁
      BiasedLocking::Condition cond = BiasedLocking::revoke_and_rebias(obj, attempt_rebias, THREAD);
      // 如果获取成功则返回，否则执行下面的slow_enter
      if (cond == BiasedLocking::BIAS_REVOKED_AND_REBIASED) {
        return;
      }
    } else { // 如果不在安全点
      assert(!attempt_rebias, "can not rebias toward VM thread");
      // 撤销偏向锁
      BiasedLocking::revoke_at_safepoint(obj);
    }
    assert(!obj->mark()->has_bias_pattern(), "biases should be revoked by now");
  }
  // 进入获取轻量级锁的过程
  slow_enter(obj, lock, THREAD);
}
```

关于偏向锁的实现可以参考[知乎问答](https://www.zhihu.com/question/55075763)中R大的回答。



本节参考链接：

https://segmentfault.com/a/1190000017255226



Synchronized是可重入的，锁关联了一个计数器，当一个线程后去了该锁时，计数器会加1，释放锁会减1，当为0时，置锁的线程标记为null，表明其他线程又可以抢锁。

## 锁升级

synchronized默认会使用偏向锁，当第二个线程参与竞争，则进入轻量级，该锁的争抢也是通过CAS来实现的。



## 锁消除和锁粗化



## volatile

volatile解决了线程间的可见性和指令重排序的问题，但是不能保证原子性。

volatile修饰变量，其实在字节码中并没有体现出特殊之处，而是在字段属性中多了一个标识为：ACC_VOLATILE。JVM解释器根据是否有该标志，然后调用不同的底层函数。

### volatile怎么保证线程间的可见性

首先来了解一下缓存的写方式：

* 写回（write back）：尽写到cache，一般当随着cache line写回主存。
* 写通（write through）：直接写回主存。
* 写失效（write invalid）：如果其他cache中有该数据，那么将其标记为失效
* 写更新（write update) ：如果其他cache中有该数据，那么将其更新到一致状态。

<font color="red">后面继续补充这一方面，CPU如何提高缓存写的效率？</font>

### volatile怎么禁止指令重排序

同样是使用lock指令，在hotspot中，后面的操作为一条空操作：为某个寄存器的值加0。x86架构有专用的内存屏障指令，但是hotspot并没有使用，使用lock指令能够兼容很多平台。

<font color="red">lock指令如何实现内存屏障的？</font>

### volatile为什么不能保证原子性

假如线程A读取了变量的值，保存到寄存器中正准备运算操作，但是被中断，所有寄存器的值都被保存（保存现场），然后另外一个线程B同样读取了该变量，对其做了修改并写回主存。之后，线程A又被重新调度时，寄存器的值就已经是脏数据了，此时再进行运算，然后写回主存的值就没有意义。





参考：

https://medium.com/@dredwardhyde/how-volatile-works-on-jvm-level-7a250d38435d

## happens-before和as-if-serial

指令重排序在编译器级别和CPU级别都存在。

as-if-serial语义保证的是单线程的情况下，指令重排，不会导致运行结果的区别。

Java规定了8条happens-before原则，可以在一定程度上解决指令重排序。

## 伪共享

不同的两个变量在同一个缓存行内，但是当其中一个变量修改了，导致整个缓存行失效，从而导致另一个变量跟着失效，这种现象称为伪共享（falsy sharing），解决办法是让其单独占有一个缓存行。

* 字节填充法
* @sun.misc.Contended 注解



## 协程

目前Java并没有原生的协程支持，一些类库已支持，比如Quasar，Loom项目目前正准备解决这个问题。