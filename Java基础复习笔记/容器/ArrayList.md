# 新建列表



**ArrayList**有几个关键属性字段：

```java
private static final int DEFAULT_CAPACITY = 10;
private static final Object[] EMPTY_ELEMENTDATA = {};
private static final Object[] DEFAULTCAPACITY_EMPTY_ELEMENTDATA = {};
transient Object[] elementData; // non-private to simplify nested class access
private int size;
```

其中==size==字段保存的是列表中有效数据的个数（等于列表的逻辑长度，不一定等于物理长度），==elementData==数组是真正保存数据的空间。==EMPTY_ELEMENTDATA==和==DEFAULTCAPACITY_EMPTY_ELEMENTDATA==的长度都为0，但是前者是在明确指定初始大小为0的情况下创建的列表所共同指向的数组，而后者是希望扩容到==DEFAULT_CAPACITY==的列表所共同指向的数组。如果指定非零初始大小值，那么==elementData==的长度就是该值。如果使用无参构造器，那么==elementData==就被指向==DEFAULTCAPACITY_EMPTY_ELEMENTDATA==。可以从下列构造方法源码中获得证据：

```java
    public ArrayList() {
        this.elementData = DEFAULTCAPACITY_EMPTY_ELEMENTDATA;
    }

    public ArrayList(int initialCapacity) {
        if (initialCapacity > 0) {
            this.elementData = new Object[initialCapacity];
        } else if (initialCapacity == 0) {
            this.elementData = EMPTY_ELEMENTDATA;
        } else {
            throw new IllegalArgumentException("Illegal Capacity: "+
                                               initialCapacity);
        }
    }
```



# 扩容机制

**ArrayList**在添加元素之前，都会检测空间是否足够。不管是**add**还是**addAll**方法，在空间不够时，都会调用grow方法来扩容，有两个**grow**方法，一个有参，一个无参：

```java
private Object[] grow() {
    return grow(size + 1);
}

private Object[] grow(int minCapacity) {
    return elementData = Arrays.copyOf(elementData,
                                       newCapacity(minCapacity));
}
```

第一个方法也是调用的第二个有参的，可以看到第一个增长幅度为1。当然这不是最终的扩容量。关键在于第二个方法内部，调用了==newCapacity==方法：

```java
private int newCapacity(int minCapacity) {
    // overflow-conscious code
    int oldCapacity = elementData.length;
    int newCapacity = oldCapacity + (oldCapacity >> 1);
    if (newCapacity - minCapacity <= 0) {
        if (elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA)
            return Math.max(DEFAULT_CAPACITY, minCapacity);
        if (minCapacity < 0) // overflow
            throw new OutOfMemoryError();
        return minCapacity;
    }
    return (newCapacity - MAX_ARRAY_SIZE <= 0)
        ? newCapacity
        : hugeCapacity(minCapacity);
}
```

将目前数组的长度（物理长度）扩展到1.5倍作为新容量，然后和传过来的参数进行比较：

* 如果新容量仍然小于等于指定大小的话，而且之前采用的无参构造器，如果这是第一次添加，那么返回==DEFAULT_CAPACITY==与==minCapacity==的最大值，否则返回==minCapacity==。

* 如果1.5倍的新容量大于指定大小的话，先和==MAX_ARRAY_SIZE==比较，这个一个非常非常大的正整数，只比最大整型数值小8。如果新容量比这个值小，那么就返回新容量，如果比这个值大，调用==hugeCapacity==方法进一步处理。

  ```java
  private static int hugeCapacity(int minCapacity) {
      if (minCapacity < 0) // overflow
          throw new OutOfMemoryError();
      return (minCapacity > MAX_ARRAY_SIZE)
          ? Integer.MAX_VALUE
          : MAX_ARRAY_SIZE;
  }
  ```

  其实这里的处理逻辑也不复杂，如果请求空间的长度确实比==MAX_ARRAY_SIZE==还大，那么就干脆直接返回==Integer.MAX_VALUE==，反正离溢出也不远了。

注意， 有两个地方判断了会溢出：

* 第一个地方是==newCapacity==内部，==newCapacity - minCapacity <= 0==与==minCapacity < 0==同时成立，说明==newCapacity<0== 满足，这是1.5倍后导致的。
* 第二个地方同样要求==minCapacity < 0==成立，为什么传入的参数会是一个小于0的数呢？三个==add==方法都是调用的无参的==grow==方法，是绝对不会出现这种情况的，刚好，==addAll==方法内部就传入了==minCapacity==值，这个传入的值是两个容器的size之和，这就有可能导致溢出。

# 遍历

**ArrayList**有两个迭代器内部类，分别是**Itr**和**ListItr**，前者实现了**Iterator**接口，后者实现了**ListIterator**（继承了**Iterator**接口）接口，并继承了前者。其实**ListItr**在**Itr**的基础上，还实现了**ListIterator**定义的操作，包括向前遍历，因为**Itr**只有向后遍历。如果要获取普通**Iterator**变量，只需要通过调用**iterator**方法就可以，如果要获得**ListItr**对象，需要调用**ListIterator**方法，这个方法有两个实现，一个无参一个有参，参数可以指定迭代器的初始下标，否则默认为0。

迭代器也是一种设计模式，对于不同的集合类，有不同的数据结构，对应的遍历操作也不同，迭代器模式可以提供同一的遍历接口，然后再由具体的类实现，不仅保护了集合内部的结构，又实现了遍历操作的统一。

但是使用迭代器时，有些必须注意的地方。比如，同一个容器对象可能同时存在多个迭代器，而这些不同的迭代器又有着不同状态，如果其中某个迭代器发生**结构性改变**(添加或者删除元素)，那么势必会影响到其他迭代器的工作。那么ArrayList是如何解决的呢？还有一点就是常听说在for-each循环里面不要进行==remove==操作，这又是为什么呢？

这里以**Itr**为例分析，对于**ListLtr**原理也是一样，只不过功能多一些而已。

**Itr**内部有三个属性：

```java
int cursor;       // index of next element to return
int lastRet = -1; // index of last element returned; -1 if no such
int expectedModCount = modCount;
```

==cursor==和==lastRet==分别是下一个要返回的对象和上一次返回的对象。==lastRet==的初始值为-1，表示没返回过任何对象。

其中==modCount==是在**AbstractList**里面定义的一个属性(注意理解注释)：

```java
/**
 * The number of times this list has been <i>structurally modified</i>.
 * Structural modifications are those that change the size of the
 * list, or otherwise perturb it in such a fashion that iterations in
 * progress may yield incorrect results.
 *
 * <p>This field is used by the iterator and list iterator implementation
 * returned by the {@code iterator} and {@code listIterator} methods.
 * If the value of this field changes unexpectedly, the iterator (or list
 * iterator) will throw a {@code ConcurrentModificationException} in
 * response to the {@code next}, {@code remove}, {@code previous},
 * {@code set} or {@code add} operations.  This provides
 * <i>fail-fast</i> behavior, rather than non-deterministic behavior in
 * the face of concurrent modification during iteration.
 *
 * <p><b>Use of this field by subclasses is optional.</b> If a subclass
 * wishes to provide fail-fast iterators (and list iterators), then it
 * merely has to increment this field in its {@code add(int, E)} and
 * {@code remove(int)} methods (and any other methods that it overrides
 * that result in structural modifications to the list).  A single call to
 * {@code add(int, E)} or {@code remove(int)} must add no more than
 * one to this field, or the iterators (and list iterators) will throw
 * bogus {@code ConcurrentModificationExceptions}.  If an implementation
 * does not wish to provide fail-fast iterators, this field may be
 * ignored.
 */
protected transient int modCount = 0;
```

注释中的信息很丰富，如果**AbstractList**的子类需要提供自定义迭代器，该字段可以用来检测并发修改（注意修改的是结构），所有实现应该在每次发生结构性变化的时候就对该字段加1操作，注意不能加太多，否则会导致假的并发修改异常。列表的==add==、==set==、==remove==方法以及它们的重写或重载版本，迭代器的==next==、==previous==（如果有）会使用到这个变量。

**Itr**类的==next==方法如下：

```java
public E next() {
    checkForComodification();
    int i = cursor;
    if (i >= size)
        throw new NoSuchElementException();
    Object[] elementData = ArrayList.this.elementData;
    if (i >= elementData.length)
        throw new ConcurrentModificationException();
    cursor = i + 1;
    return (E) elementData[lastRet = i];
}
```

该方法第一句就调用了一个方法--==checkForComodification==:

```java
final void checkForComodification() {
    if (modCount != expectedModCount)
        throw new ConcurrentModificationException();
}
```

==modCount==是一个**ArrayList**对象所拥有的属性，==expectedModCount==是一个**Iterator**对象的属性，而一个数组列表对象又可以拥有多个迭代器，如果多个迭代器并发迭代列表，而此时会影响到其他的迭代器的迭代过程。这称作为**快速失败**实现，<font color=red>但是千万要注意，这种方案并不能真正做到线程安全</font>。比如在==checkForComodification==验证通过后，当前线程可能被暂停允许，由其他线程继续运行可能产生结构性变化，从而导致错误结果。所以请不要在多线程环境下直接使用**ArrayList**。

假如就在单线程环境下，为什么不要在==for-each==中进行==remove==操作呢？

```java
public static void main(String[] args) {
    var list = new ArrayList<>(Arrays.asList(1,2,3,4,5));
    for(Integer i : list) {
        list.remove(i);
    }
}
```

这是一段简单的测试代码，不出所料，果然发生了异常，反编译一下代码：

```java
public static void main(String args[])
{
    ArrayList list = new ArrayList(Arrays.asList(new Integer[] {
        Integer.valueOf(1), Integer.valueOf(2), Integer.valueOf(3), Integer.valueOf(4), 					Integer.valueOf(5)
    }));
    Integer i;
    for(Iterator iterator = list.iterator(); iterator.hasNext(); list.remove(i))
        i = (Integer)iterator.next();
}
```

可以看到代码中的==for-each==转为了普通的==for==循环，但是靠迭代器来控制的。

被调用的==remove==方法的代码为：

```java
public boolean remove(Object o) {
    final Object[] es = elementData;
    final int size = this.size;
    int i = 0;
    found: {
        if (o == null) {
            for (; i < size; i++)
                if (es[i] == null)
                    break found;
        } else {
            for (; i < size; i++)
                if (o.equals(es[i]))
                    break found;
        }
        return false;
    }
    fastRemove(es, i);
    return true;
}
```

又调用了==fastRemove==方法，代码如下：

```java
private void fastRemove(Object[] es, int i) {
    modCount++;
    final int newSize;
    if ((newSize = size - 1) > i)
        System.arraycopy(es, i + 1, es, i, newSize - i);
    es[size = newSize] = null;
}
```

代码进入==for==循环，先调用了==next==方法，此时迭代器内部的数据的值如下：

```java
cursor=1;
lastRet=0;
expectedModCount=0
```

但是紧接在着下面调用==remove==方法，直接调用的是**ArrayList**对象的，而不是迭代器对象的==remove==方法，然后又调用了==fastRemove==方法，这个方法第一句就修改了==modCount==的值，然后修改完成就返回了。当下一轮再执行==next==方法的时候，调用==checkForComodification==方法，检测==modCount==和==expectedModCount==是否相等，明显不等，因为==modCount==的值为1，而==expectedModCount==的值为0。

上面的分析告一段落，但是**remove**方法没有这么简单，看下面的例子：

```java
Iterator<Integer>iterator = list.iterator();
while(iterator.hasNext()) {
    iterator.next();
    iterator.remove();
}
```

这一个例子就能成功地运行完毕，同样使用迭代器，同样也是先调用==next==方法，再调用==remove==方法，但是这里和上面不同的是，这里的==remove==方法是通过迭代器调用的，而上面是调用的列表对象的==remove==方法。下面看一下**Itr**内部的==remove==方法源码：

```java
public void remove() {
    if (lastRet < 0)
        throw new IllegalStateException();
    checkForComodification();

    try {
        ArrayList.this.remove(lastRet);
        cursor = lastRet;
        lastRet = -1;
        expectedModCount = modCount;
    } catch (IndexOutOfBoundsException ex) {
        throw new ConcurrentModificationException();
    }
}
```

发现内部调用了**ArrayList**实现的==remove==方法，而且和上面那个例子中调用的==remove==方法还不一样（一个参数为对象，一个参数为下标）：

```java
public E remove(int index) {
    Objects.checkIndex(index, size);
    final Object[] es = elementData;

    @SuppressWarnings("unchecked") E oldValue = (E) es[index];
    fastRemove(es, index);

    return oldValue;
}
```

同样，这里面调用了==fastRemove==方法，源码和上面一样。但是注意迭代器内部的==remove==方法的第10行：

```java
expectedModCount = modCount;
```

<font color=red>这里同步了两个值，这就是差异</font>。迭代器内部的==remove==方法调用外部的==remove==方法时，外部的==remove==方法一个参数是==int==型的，另一个参数是==Object==型的，为什么==int==变量不会自动装箱到==Integer==型然后调用参数为==Object==的那个方法呢？关于重载方法匹配优先级，请参考《深入理解Java虚拟机（第三版）》P306。

如果把上面第二个例子修改一下：

```java
Iterator<Integer>iterator = list.iterator();
while(iterator.hasNext()) {
    // iterator.next();
    iterator.remove();
}
```

这段代码会产生==java.lang.IllegalStateException==异常，原因在于在迭代器内部的==remove==方法中，首先检测了==lastRet==的值，如果小于0，就会抛出该异常。

下面这个例子同样会产生并发修改异常：

```java
Iterator<Integer> iterator = list.iterator();
list.remove(0);
while(iterator.hasNext()) {
    Integer next = iterator.next();
    System.out.println(next);
}
```

因为在执行第二句的时候，==list==的==modCount==改变了，但是迭代器声明是在第一句，也就是在改变之前就已经创建迭代器对象了，对象内部的==expectedModCount==还等于0，所以就造成不一致了。

不能分析完所有的例子，但是原理都是一样的，通过上面几个例子理解原理就行。



# 扩容机制（20200610再次分析）

这是扩容步骤的第一个关键方法：

```java
private Object[] grow(int minCapacity) {
    return elementData = Arrays.copyOf(elementData,
                                       newCapacity(minCapacity));
}
```

这里的**minCapacity**有没有可能小于0，也即是说溢出了？当然可能，不管是**add**方法还是**addAll**方法，都没有判断是否溢出了(但是不可能等于0)。继续看**newCapacity**方法：

```java
private int newCapacity(int minCapacity) {
    // overflow-conscious code
    int oldCapacity = elementData.length;
    int newCapacity = oldCapacity + (oldCapacity >> 1);
    if (newCapacity - minCapacity <= 0) {
        if (elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA)
            return Math.max(DEFAULT_CAPACITY, minCapacity);
        if (minCapacity < 0) // overflow
            throw new OutOfMemoryError();
        return minCapacity;
    }
    return (newCapacity - MAX_ARRAY_SIZE <= 0)
        ? newCapacity
        : hugeCapacity(minCapacity);
}
```

可见，这里判断了溢出的情况，但是哪些情况下会造成溢出呢？这里的**minCapacity**和**newCapacity**都是有可能小于0的。

整个方法根据条件**newCapacity - minCapacity <= 0**分为了两个分支：

第一个分支，要满足这个条件，可能有以下几种情况：



首先看第一个分支，要满足这个条件，有以下几种情况（对应了内部的几种分支）：

1. newCapacity <= minCapacity < 0
2. newCapacity < 0 < minCapacity
3. 0 < newCapacity  <= minCapacity

从第一个子分支可以看出，如果该列表对象是通过无参构造方法创建的，那么在第一次执行**add**操作时，会扩容到**DEFAULT_CAPACITY**大小，即10。如果执行的是**addAll**，那么则根据传入的集合大小与10进行比较，取较大者。这个子分支毫无疑问会满足第3种情况。

第二个子分支判断了**minCapacity**是否小于0，对应了第1种情况，那么直接抛出异常。

第三个子分支属于第2种或第3种情况，即1.5倍的扩容策略不行了，那么就进行按需扩容。



第二个分支，首先满足newCapacity > minCapacity，其次，再加上一个条件newCapacity <= MAX_ARRAY_SIZE

