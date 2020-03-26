以下内容基于JDK11，有些文字来自官方API的翻译，另外是一些自己的理解。

# Queue

这是一个队列接口，只定义常规队列的基本操作，注意不是所有的队列都是FIFO（先进先出）的，还可能是双端队列、优先级队列等。

```java
public interface Queue<E> extends Collection<E> {
	boolean add(E e);
    boolean offer(E e);
    
    E remove();
    E poll();
    
    E element();
    E peek();
}
```

这六个方法分成三组，每两个一组，每组提供的功能都会一致，但是有些差异。

## add & offer

这两个方法是往队列中添加元素，但是可能抛出下面几种异常：

* ==ClassCastException==：如果参数e的类型不能被加入到队列中，会抛出这种异常。
* ==NullPointerException==：如果队列中不允许存在**null**值，而又准备添加**null**值时，就会抛出这种异常。
* ==IllegalArgumentException==：如果参数e对象的某些属性导致不能被加入到队列中，会抛出这种异常。
* ==IllegalStateException==：<font color=red>注意，这是==add==方法和==offer==方法的差异：</font>当队列已满时，==add==方法会抛出该异常，但是==offer==方法不会。在需要提供空间严格性队列的时候，最好使用==add==方法，因为会第一时间抛出异常。

如果成功添加元素进入队列，那么这两个方法返回**true**，否则返回**false**。

## remove & poll

这两个方法都对队列执行出队操作（对头出队），但是当队列为空时，==remove==方法会抛出==NoSuchElementException==异常，而==poll==方法只会返回**null**。

## element & peak

这两个方法都是返回对头元素，<font color=red>而不执行出队操作。</font>如果队列为空，==element==方法会抛出==NoSuchElementException==异常，但是==peak==方法则不会。



# AbstractQueue

这是一个抽象类，实现**Queue**接口，并提供了下面几个方法的大概实现。

```java
public abstract class AbstractQueue<E>
    extends AbstractCollection<E>
    implements Queue<E> {
    
    protected AbstractQueue() {
    }
    
    public boolean add(E e) {
        if (offer(e))
            return true;
        else
            throw new IllegalStateException("Queue full");
    }
    
    public E remove() {
        E x = poll();
        if (x != null)
            return x;
        else
            throw new NoSuchElementException();
    }    
    
    public E element() {
        E x = peek();
        if (x != null)
            return x;
        else
            throw new NoSuchElementException();
    }
    
    public void clear() {
        while (poll() != null)
            ;
    }
    
    public boolean addAll(Collection<? extends E> c) {
        if (c == null)
            throw new NullPointerException();
        if (c == this)
            throw new IllegalArgumentException();
        boolean modified = false;
        for (E e : c)
            if (add(e))
                modified = true;
        return modified;
    }
}
```

其中，==add==，==remove==，==element==三个方法是实现的**Queue**接口的，而且这三个方法里面，都是基于对应的==offer==，==poll==，==peak==方法实现的，然后根据返回值来决定是否抛出异常。这个抽象类不允许队列内部有**null**元素，继承该抽象类的具体类至少要实现==offer==，==poll==，==peak==, ==size==，==iterator==这几个方法，而且在==offer==中不允许插入空值。

## clear

该方法实现也很简单，就是**while**循环调用==poll==方法，直到返回false，说明已清空完毕。

## addAll

注意，该方法的实现不允许添加自身或空集合，否则分别抛出代码中所示的两种异常。该方法基于**for-each**循环，然后调用**add**方法来添加元素。



# ArrayQueue

后面补充。

# Deque

这是一个双端队列接口，允许在两端进行出队入队操作，**Deque**就是"double ended queue"的简写。该接口的大部分实现对元素数量没有限制，但也可以提供容量限制性队列实现。



## Summary of Deque methods

对于insert, remove, examine操作，该接口定义了两组方法，一组是在某些失败原因情况下会抛出异常，另一组是不会抛异常，但是会返回特定值，如**null**。

![image-20200303111515430](C:\Users\HarrisonLee\AppData\Roaming\Typora\typora-user-images\image-20200303111515430.png)

上面方法的靠方法名就能得知是什么意思，其中==getFirst==和==getLast==类似**Queue**的==element==方法一样，返回元素而不剔除元素。

## Comparison of Queue and Deque methods

**Deque**接口继承与**Queue**接口，当**Deque**当作一般队列使用时，**FIFO**行为将会产生，在**Queue**中定义的方法完全等价于在**Deque**中定义的一些方法，对应关系如下图：

![image-20200303112320321](C:\Users\HarrisonLee\AppData\Roaming\Typora\typora-user-images\image-20200303112320321.png)



## Comparison of Stack and Deque methods

另外，**Deque**还提供了栈的功能，即**LIFO（后进先出）**特性，应该使用这种方式而不要使用传统的**Stack**，因为其基于**vector**实现的。当作为栈来使用的时候，统一在队列头部进行入栈出栈操作。方法对应关系如下图所示：

![image-20200303112916975](C:\Users\HarrisonLee\AppData\Roaming\Typora\typora-user-images\image-20200303112916975.png)

# ArrayDeque

注意和上面的**ArrayQueue**区分，**ArrayDeque**是**Deque**的实现类：

```java
public class ArrayDeque<E> extends AbstractCollection<E>
                           implements Deque<E>, Cloneable, Serializable
```

它是**Deque**的一种非固定空间大小的实现，是可以更改空间大小的。也就说明和**ArrayList**一样，能够自动扩容。不是线程安全的，不支持同步操作。不准插入**null**值。当作为队列来使用时，效率比**LinkedList**高，当作为栈时，效率比**Stack**高。

**ArrayDeque**定义的大部分操作运行时间都是常数级，以下几个是线性增长的：

* remove(Object)
* removeFirstOccurrence
* removeLastOccurrence
* contains
* iterator.remove
* 以及上面方法对应的批处理方法（如果存在）

和**ArrayList**一样，采用**fail-fast**机制来检测并发修改异常。在迭代器创建后的任何时候，采用非迭代器内部的==remove==方法来修改对象的话，那么将会抛出==ConcurrentModificationException==异常。但是这种机制也是尽力而为，不能真正确保线程安全。

## 属性字段

### elements

```java
transient Object[] elements;
```

这个字段是用来保存元素的数组，任何不持有元素的对象的该字段都为**null**。在有元素存储时，该数组总是最后至少存在一个空槽，值为**null**。

### head

```java
transient int head;
```

该字段保存队头元素在==elements==数组中的下标，所谓对头元素就是会被==remove==或==pop==方法移除的那个元素。如果队列为空（数组可能不为空），那么该值为[0, elements.length())区间中的任意整数值。

### tail 

```java
transient int tail;
```

该值是下一次添加元素时的元素的下标，注意<font color=red>elements[tail] is always null</font>。

### MAX_ARRAY_SIZE

```java
private static final int MAX_ARRAY_SIZE = Integer.MAX_VALUE - 8;
```

和**ArrayList**一样，**ArrayDeque**同样有这个属性。

## 构造器

```java
public ArrayDeque() {
    elements = new Object[16];
}
```

无参构造器，那么会数组会默认开辟16个长度的空间。

```java
public ArrayDeque(int numElements) {
    elements =
        new Object[(numElements < 1) ? 1 :
                   (numElements == Integer.MAX_VALUE) ? Integer.MAX_VALUE :
                   numElements + 1];
}
```

传入一个值，表示初始化元素的数量。另外对==numElements==的操作在代码中很清晰，不在赘述。

```java
public ArrayDeque(Collection<? extends E> c) {
    this(c.size());
    copyElements(c);
}
```

先调用上面第二个构造器， 以开辟空间，然后调用==copyElements==方法，复制元素。

```java
private void copyElements(Collection<? extends E> c) {
    c.forEach(this::addLast);
}
```

可以看到是调用的**for-each**循环，依次执行==addLast==入队。

## 入队

### 在队头入队

```java
public void addFirst(E e) {
    if (e == null)
        throw new NullPointerException();
    final Object[] es = elements;
    es[head = dec(head, es.length)] = e;
    if (head == tail)
        grow(1);
}
```

在决定插入元素的下标的时候，调用了==dec==方法：

```java
static final int dec(int i, int modulus) {
    if (--i < 0) i = modulus - 1;
    return i;
}
```

如果==head==的值已经为0了，那么下一次==head==的值就应该等于最后一个数组元素的下标。

添加元素后，如果发现==head==和==tail==相等了，那么调用==grow==方法就行扩容，后面会讲到。

### 在队尾入队

```java
public void addLast(E e) {
    if (e == null)
        throw new NullPointerException();
    final Object[] es = elements;
    es[tail] = e;
    if (head == (tail = inc(tail, es.length)))
        grow(1);
}
```

调用了==inc==方法来处理==tail==：

```java
static final int inc(int i, int modulus) {
    if (++i >= modulus) i = 0;
    return i;
}
```

### 批量入队

```java
public boolean addAll(Collection<? extends E> c) {
    final int s, needed;
    if ((needed = (s = size()) + c.size() + 1 - elements.length) > 0)
        grow(needed);
    copyElements(c);
    return size() > s;
}
```

如果空间不够就扩容，然后调用==copyElements==复制元素。在上面构造器部分，第三个构造器也是对容器批量入队，也调用了这个方法，方法内部是通过调用的==addLast==方法来实现的。

### 传统add入队

```java
public boolean add(E e) {
    addLast(e);
    return true;
}
```

调用**addLast**方法，符合传统**FIFO**特性。

### offer入队

另外，==offer==系列方法也可以入队，但是调用的都是上面的两个方法：

```java
public boolean offer(E e) {
    return offerLast(e);
}

public boolean offerFirst(E e) {
    addFirst(e);
    return true;
}

public boolean offerLast(E e) {
    addLast(e);
    return true;
}
```

这里是==offer==依靠==add==来实现，和**AbstractQueue**的实现不同，刚好相反。

### push入队

```java
public void push(E e) {
    addFirst(e);
}
```



## 扩容

**ArrayDeque**方法通过==grow==方法来实现的：

```java
private void grow(int needed) {
    // overflow-conscious code
    final int oldCapacity = elements.length;
    int newCapacity;
    // Double capacity if small; else grow by 50%
    int jump = (oldCapacity < 64) ? (oldCapacity + 2) : (oldCapacity >> 1);
    if (jump < needed
        || (newCapacity = (oldCapacity + jump)) - MAX_ARRAY_SIZE > 0)
        newCapacity = newCapacity(needed, jump);
    final Object[] es = elements = Arrays.copyOf(elements, newCapacity);
    // Exceptionally, here tail == head needs to be disambiguated
    if (tail < head || (tail == head && es[head] != null)) {
        // wrap around; slide first leg forward to end of array
        int newSpace = newCapacity - oldCapacity;
        System.arraycopy(es, head,
                         es, head + newSpace,
                         oldCapacity - head);
        for (int i = head, to = (head += newSpace); i < to; i++)
            es[i] = null;
    }
}
```

内部有一个分支调用了==newCapacity==方法：

```java
private int newCapacity(int needed, int jump) {
    final int oldCapacity = elements.length, minCapacity;
    if ((minCapacity = oldCapacity + needed) - MAX_ARRAY_SIZE > 0) {
        if (minCapacity < 0)
            throw new IllegalStateException("Sorry, deque too big");
        return Integer.MAX_VALUE;
    }
    if (needed > jump)
        return minCapacity;
    return (oldCapacity + jump - MAX_ARRAY_SIZE < 0)
        ? oldCapacity + jump
        : MAX_ARRAY_SIZE;
}
```

这里的==Capactity==和**ArrayList**中的**Capacity**方法行为类似，这里传入两个参数，第一个是确确实实需要的量，第二个是依赖于当前数组的长度的量，在==grow==方法的第6行中定义，如果==jump==满足不了==needed==或者是旧容量大小加上==jump==大于了==MAX_ARRAY_SIZE==, 就会调用==newCapacity==方法，进一步判断，如果==oldCapacity + needed==溢出了，说明需要的容量太大了，无法满足，直接抛出异常，否则返回==Integer==的最大值（其实离溢出也不远了）。如果容量需要没那么大，而且需求量大于==jump==值，直接返回旧容量加上需求量，否则比较旧容量加上==jump==值与==MAX_ARRAY_SIZE==。

说了这么多，其实在需求量没那么大的情况下，如果需求量小于系统提供的扩容量，那么就按照推荐的==jump==值大小进行扩容，否则就按照实际需要多少就扩容多少。

==grow==返回得到扩容量后，调用**Arrays.copyOf**复制数组。如果出现意外情况（两种情况）：

* ==tail < head==：队尾在前，对头在后
* ==tail == head && es[head] != null==：当“tail\==head”时，有两种情况，一是队列为空，二是队列已满。上面讲到**ArrayDeque**对象的elements数组必须要求有一个空值（tail下标表示的那个位置）。那么这里属性不为空，就说明被新加入的元素覆盖掉了，调用**System.arraycopy**复制数组，然后修改==head==的值, 并把之前被移走的空间赋值为**null**。

*确实复杂，慢慢分析~*

## 移除

移除并返回被移除的对象。

### 队头出队

```java
public E pollFirst() {
    final Object[] es;
    final int h;
    E e = elementAt(es = elements, h = head);
    if (e != null) {
        es[h] = null;
        head = inc(h, es.length);
    }
    return e;
}
```

### 队尾出队

```java
public E pollLast() {
    final Object[] es;
    final int t;
    E e = elementAt(es = elements, t = dec(tail, es.length));
    if (e != null)
        es[tail = t] = null;
    return e;
}
```

### 默认出队对头

```java
public E poll() {
    return pollFirst();
}
```

默认调用的是==pollFirst==，符合传统的**FIFO**特性。

### remove出队

```java
public E remove() {
    return removeFirst();
}

public E removeFirst() {
    E e = pollFirst();
    if (e == null)
        throw new NoSuchElementException();
    return e;
}

public E removeLast() {
    E e = pollLast();
    if (e == null)
        throw new NoSuchElementException();
    return e;
}
```

### pop出队

```java
public E pop() {
    return removeFirst();
}
```



### 移除给定对象第一次出现的元素

给定一个对象，根据特定顺序，移除遍历到该对象第一次出现的元素。

#### 从头到尾

```java
public boolean removeFirstOccurrence(Object o) {
    if (o != null) {
        final Object[] es = elements;
        for (int i = head, end = tail, to = (i <= end) ? end : es.length;
             ; i = 0, to = end) {
            for (; i < to; i++)
                if (o.equals(es[i])) {
                    delete(i);
                    return true;
                }
            if (to == end) break;
        }
    }
    return false;
}
```

#### 从尾到头

```java
public boolean removeLastOccurrence(Object o) {
    if (o != null) {
        final Object[] es = elements;
        for (int i = tail, end = head, to = (i >= end) ? end : 0;
             ; i = es.length, to = end) {
            for (i--; i > to - 1; i--)
                if (o.equals(es[i])) {
                    delete(i);
                    return true;
                }
            if (to == end) break;
        }
    }
    return false;
}
```

这里面都调用了==delete==方法：

```java
boolean delete(int i) {
    final Object[] es = elements;
    final int capacity = es.length;
    final int h, t;
    // number of elements before to-be-deleted elt
    final int front = sub(i, h = head, capacity);
    // number of elements after to-be-deleted elt
    final int back = sub(t = tail, i, capacity) - 1;
    if (front < back) {
        // move front elements forwards
        if (h <= i) {
            System.arraycopy(es, h, es, h + 1, front);
        } else { // Wrap around
            System.arraycopy(es, 0, es, 1, i);
            es[0] = es[capacity - 1];
            System.arraycopy(es, h, es, h + 1, front - (i + 1));
        }
        es[h] = null;
        head = inc(h, capacity);
        return false;
    } else {
        // move back elements backwards
        tail = dec(t, capacity);
        if (i <= tail) {
            System.arraycopy(es, i + 1, es, i, back);
        } else { // Wrap around
            System.arraycopy(es, i + 1, es, i, capacity - (i + 1));
            es[capacity - 1] = es[0];
            System.arraycopy(es, 1, es, 0, t - 1);
        }
        es[tail] = null;
        return true;
    }
}
```



## 返回元素

```java
static final <E> E elementAt(Object[] es, int i) {
    return (E) es[i];
}
```

这个==elementAt==方法是所有获取元素的公共方法。

### 返回队头元素

```java
public E getFirst() {
    E e = elementAt(elements, head);
    if (e == null)
        throw new NoSuchElementException();
    return e;
}

public E peekFirst() {
    return elementAt(elements, head);
}

public E peek() {
    return peekFirst();
}
```

如果队列为空，第一个方法要抛出异常，而第二个返回**null**值。

### 返回队尾元素

```java
public E getLast() {
    final Object[] es = elements;
    E e = elementAt(es, dec(tail, es.length));
    if (e == null)
        throw new NoSuchElementException();
    return e;
}

public E peekLast() {
    final Object[] es;
    return elementAt(es = elements, dec(tail, es.length));
}
```

如果队列为空，第一个方法要抛出异常，而第二个返回**null**值。

### element返回

**Deque**的==element==方法对应**ArrayDeque**的==getFirst==方法：

```java
public E element() {
    return getFirst();
}
```



### 迭代器

后面补充

