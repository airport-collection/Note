# 并发基础

## CAS和AQS

什么是CAS？什么又是AQS？



CAS怎么解决ABA问题？版本号，时间戳

CAS底层是如何保证原子性的？加Lock指令 https://www.hicsc.com/post/2020031059



## 锁升级



## Synchronized

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

## Volatile



## 伪共享

* 字节填充法
* @sun.misc.Contended 注解

