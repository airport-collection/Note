

允许**null**值和**null**键，其中，允许最多一个键为null, 多个值为null。不保存元素的添加顺序。

**HashMap**代码非常多，在Java11的版本中，达到2444行（包括注释）。同时，其内部类也非常多，一共13个,分别是：

```java
static class Node<K,V> implements Map.Entry<K,V> 
final class KeySet extends AbstractSet<K>
final class Values extends AbstractCollection<V>
final class EntrySet extends AbstractSet<Map.Entry<K,V>>
abstract class HashIterator
final class KeyIterator extends HashIterator implements Iterator<K>
final class ValueIterator extends HashIterator implements Iterator<V>
final class EntryIterator extends HashIterator implements Iterator<Map.Entry<K,V>> 
static class HashMapSpliterator<K,V>
static final class KeySpliterator<K,V> extends HashMapSpliterator<K,V> implements Spliterator<K> 
static final class ValueSpliterator<K,V> extends HashMapSpliterator<K,V> implements Spliterator<V>
static final class EntrySpliterator<K,V> extends HashMapSpliterator<K,V> implements pliterator<Map.Entry<K,V>> 
static final class TreeNode<K,V> extends LinkedHashMap.Entry<K,V>
```

#  属性字段



```java
static final int DEFAULT_INITIAL_CAPACITY = 1 << 4;
```

默认的初始容量为16。



```java
static final int MAXIMUM_CAPACITY = 1 << 30;
```

最大容量，如果通过构造器隐含指定了更大的值，那么会自动降低为该值。容量必须是小于等于该值而且是2的幂。
如果忽略了必须是2的幂的话，那么可能会产生为什么不是(1<<31)-1的疑问？虽然Integer.MAX_VALUE是int型的最大值，但是其不是2的幂。
那么为什么必须是2的幂呢？
为了简化实现和提高性能：比如查找元素在hash标中的元素时，可以使用**hash & (SIZE -1)**而不是**abs(hash) % SIZE**。另外，2的幂的二进制表示法中只有第k位为1，[1,k)位都为0，那么SIZE-1后，第[k, 32]位为0，第[0, k)都为1，对于小于等于**SIZE**的hash值来说，与运算后的结果就是自身，只要hash值能做到不同，那么就尽可能地减少了碰撞的几率。如果通过构造器指定了不是2的幂的容量，那么会自动调用**tableSizeFor**方法修正这个数值。



```java
static final float DEFAULT_LOAD_FACTOR = 0.75f;
```

默认的负载因子，如果构造器中没有指定，则只用该值。为什么该值是**0.75**？负载因子是决定何时自动增加容量的，如果负载因子太小，则会导致频繁扩容，造成时间开销增大甚至空间浪费。如果负载因子太大，那么可能造成hash冲突机率增大，不利于查找数据。

该值是可以大于1的，当大于1的时候，可能会加剧hash冲突，从而形成更多更长的链表，增加查找的复杂度。

<font color="red">那为什么偏偏是0.75，而不是0.74或0.76呢？</font>

官方文档给出使用**0.75**的原因是时间和空间开销的折中。但具体为什么是0.75,目前我尚未得知，但查阅到一篇非常不错的文章：https://preshing.com/20110504/hash-collision-probabilities/。



```java
static final int TREEIFY_THRESHOLD = 8;
```

列表结构转化为树结构的临界值，也就是说当桶中的节点数达到8个时，那么就会触发转型为树型结构。



```java
static final int UNTREEIFY_THRESHOLD = 6;
```



```java
static final int MIN_TREEIFY_CAPACITY = 64;
```



```java
transient Node<K,V>[] table;
```



```java
transient Set<Map.Entry<K,V>> entrySet;
```



```java
transient int size;
```



```java
transient int modCount;
```



```java
int threshold;
```



```java
final float loadFactor;
```



# 构造方法

下面对其四个构造器进行编号

A构造器

```java
public HashMap() {
    this.loadFactor = DEFAULT_LOAD_FACTOR; // all other fields defaulted
}
```

B构造器

```java
public HashMap(int initialCapacity) {
    this(initialCapacity, DEFAULT_LOAD_FACTOR);
}
```

C构造器

```java
public HashMap(int initialCapacity, float loadFactor) {
    if (initialCapacity < 0)
        throw new IllegalArgumentException("Illegal initial capacity: " +
                                           initialCapacity);
    if (initialCapacity > MAXIMUM_CAPACITY)
        initialCapacity = MAXIMUM_CAPACITY;
    if (loadFactor <= 0 || Float.isNaN(loadFactor))
        throw new IllegalArgumentException("Illegal load factor: " +
                                           loadFactor);
    this.loadFactor = loadFactor;
    this.threshold = tableSizeFor(initialCapacity);
}
```

D构造器

```java
public HashMap(Map<? extends K, ? extends V> m) {
    this.loadFactor = DEFAULT_LOAD_FACTOR;
    putMapEntries(m, false);
}
```

其核心在于C构造器，其内部调用了**tableSizeFor**方法来确定容量上限,这个方法是返回==不小于给定值且最接近于它的2的幂==，但它是怎么做到的呢？其源码如下：

```java
static final int tableSizeFor(int cap) {
    int n = -1 >>> Integer.numberOfLeadingZeros(cap - 1);
    return (n < 0) ? 1 : (n >= MAXIMUM_CAPACITY) ? MAXIMUM_CAPACITY : n + 1;
}
```

该方法调用了**numberOfLeadingZeros**，其源码如下：

```java
@HotSpotIntrinsicCandidate
public static int numberOfLeadingZeros(int i) {
    // HD, Count leading 0's
    if (i <= 0)
        return i == 0 ? 32 : 0;
    int n = 31;
    if (i >= 1 << 16) { n -= 16; i >>>= 16; }
    if (i >= 1 <<  8) { n -=  8; i >>>=  8; }
    if (i >= 1 <<  4) { n -=  4; i >>>=  4; }
    if (i >= 1 <<  2) { n -=  2; i >>>=  2; }
    return n - (i >>> 1);
}
```

该方法的作用是查找某个数的二进制表示法中，从最高位到最低为方向上连续0的个数（其方法名很形象）。其查找过程是典型的二分查找，从那四个if中就可以看出来。

-1的补码为：11111111 11111111 11111111 11111111

注意全是1（非常巧妙），然后原来的数的二进制高位有m个0，那么也将-1无符号左移m位，那么结果是剩下的32-m位全是1，然后再执行**n+1**, 那么得到的结果刚好就是大于等于原来的值且同时是属于2的幂。

# 存储结构

## Node

在属性中，有一个名为**table**的字段，这就是真正保存键值对的结构，**Node**是其一个内部类，代码比较简单，通过查阅源码，可以得知该结构用于形成一个链表，这也是解决hash冲突的链地址法所依赖的结构。



## TreeNode

该内部类继承了**LinkedHashMap.Entry**,而后者又继承了**HashMap.Node**, 这也是为什么在**HashMap**中的**table**属性能够保存两种结构元素的原因。

属性：

```java
// 
TreeNode<K,V> parent;  // red-black tree links
// 左子节点
TreeNode<K,V> left;
// 右子节点
TreeNode<K,V> right;
//
TreeNode<K,V> prev;    // needed to unlink next upon deletion
// 是否是红色节点
boolean red;
```



# 功能实现

## 确定元素所在hash桶的位置

不管增加、删除、查找键值对，定位到哈希桶数组的位置都是很关键的第一步。在**HashMap**中，通过调用**hash**方法来获取hash值：

```java
static final int hash(Object key) {
    int h;
    return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);
}
```

如果键不为**null**，那么将键的hashCode与其本身无符号左移16位后再异或运算得到的结果作为该键的hash值，这个值就是用来确定键所属桶在**table**中的下标。将高位无符号右移到低位主要是从速度、功效、质量来考虑的，这么做可以在数组**table**的length比较小的时候，也能保证考虑到高低Bit都参与到Hash的计算中，同时不会有太大的开销。

![](https://awps-assets.meituan.net/mit-x/blog-images-bundle-2016/45205ec2.png)

## put和resize操作

```java
public V put(K key, V value) {
    return putVal(hash(key), key, value, false, true);
}

/**
* @param onlyIfAbsent if true, don't change existing value
* @param evict if false, the table is in creation mode.
*/
final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
               boolean evict) {
    Node<K,V>[] tab; Node<K,V> p; int n, i;
    // X SECTION
    if ((tab = table) == null || (n = tab.length) == 0)
        n = (tab = resize()).length;
    // Y SECTION
    if ((p = tab[i = (n - 1) & hash]) == null)
        tab[i] = newNode(hash, key, value, null);
    else {
        Node<K,V> e; K k;
        if (p.hash == hash &&
            ((k = p.key) == key || (key != null && key.equals(k))))
            e = p;
        else if (p instanceof TreeNode)
            e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
        else {
            for (int binCount = 0; ; ++binCount) {
                if ((e = p.next) == null) {
                    p.next = newNode(hash, key, value, null);
                    if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
                        treeifyBin(tab, hash);
                    break;
                }
                if (e.hash == hash &&
                    ((k = e.key) == key || (key != null && key.equals(k))))
                    break;
                p = e;
            }
        }
        if (e != null) { // existing mapping for key
            V oldValue = e.value;
            if (!onlyIfAbsent || oldValue == null)
                e.value = value;
            afterNodeAccess(e);
            return oldValue;
        }
    }
    ++modCount;
    // Z SECTION
    if (++size > threshold)
        resize();
    afterNodeInsertion(evict);
    return null;
}
```

很明显，核心逻辑都在**putVal**中，其中有5个参数，注意最后两个的含义。

整个代码可以分为三部分，X部分为第一个if, Y部分为第二个if以及else, Z部分为最后一个if。

X部分主要是判断**table**是否为空或者是长度为0, 那么什么情况下会满足这种条件呢？

回想一下构造方法，对于A,B,C三个构造方法（暂时不考虑D），均没有对**table**进行初始化，那么说明在第一次调用**put**方法的时候会满足**table==null**。那么什么情况会出现**table.length==0**呢？请看下面的Y部分的分析。另外，A部分调用了**resize**方法：

```java
final Node<K,V>[] resize() {
    Node<K,V>[] oldTab = table;
    int oldCap = (oldTab == null) ? 0 : oldTab.length;
    int oldThr = threshold;
    int newCap, newThr = 0;
    // R1
    if (oldCap > 0) {
        if (oldCap >= MAXIMUM_CAPACITY) {
            threshold = Integer.MAX_VALUE;
            return oldTab;
        }
        else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY &&
                 oldCap >= DEFAULT_INITIAL_CAPACITY)
            newThr = oldThr << 1; // double threshold
    }
    else if (oldThr > 0) // initial capacity was placed in threshold
        newCap = oldThr;
    else {               // zero initial threshold signifies using defaults
        newCap = DEFAULT_INITIAL_CAPACITY;
        newThr = (int)(DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY);
    }
    // R2
    if (newThr == 0) {
        float ft = (float)newCap * loadFactor;
        newThr = (newCap < MAXIMUM_CAPACITY && ft < (float)MAXIMUM_CAPACITY ?
                  (int)ft : Integer.MAX_VALUE);
    }
    threshold = newThr;
    @SuppressWarnings({"rawtypes","unchecked"})
    Node<K,V>[] newTab = (Node<K,V>[])new Node[newCap];
    table = newTab;
    // R3
    if (oldTab != null) {
        for (int j = 0; j < oldCap; ++j) {
            Node<K,V> e;
            if ((e = oldTab[j]) != null) {
                oldTab[j] = null;
                if (e.next == null)
                    newTab[e.hash & (newCap - 1)] = e;
                else if (e instanceof TreeNode)
                    ((TreeNode<K,V>)e).split(this, newTab, j, oldCap);
                else { // preserve order
                    Node<K,V> loHead = null, loTail = null;
                    Node<K,V> hiHead = null, hiTail = null;
                    Node<K,V> next;
                    do {
                        next = e.next;
                        if ((e.hash & oldCap) == 0) {
                            if (loTail == null)
                                loHead = e;
                            else
                                loTail.next = e;
                            loTail = e;
                        }
                        else {
                            if (hiTail == null)
                                hiHead = e;
                            else
                                hiTail.next = e;
                            hiTail = e;
                        }
                    } while ((e = next) != null);
                    if (loTail != null) {
                        loTail.next = null;
                        newTab[j] = loHead;
                    }
                    if (hiTail != null) {
                        hiTail.next = null;
                        newTab[j + oldCap] = hiHead;
                    }
                }
            }
        }
    }
    return newTab;
}
```

这个方法是相当的长，这里暂时只找出满足**putVal**中X部分的逻辑。

对于R1部分，会执行第二、三条分支，主要区别在于**threshold**是否为0。如果采用的是A构造器，那么**threshold==0**成立，如果采用的是B或C构造器，是不会出现**threshold**为0的情况的。所以，采用A构造器，执行的是第三条分支，如果采用的是B或C构造器，执行的是第二条分支。

* 如果执行第三条分支，那么将新容量设置为默认的初始容量，即16。同时将**threshold**设置为默认的初始容量于默认的装载因子的乘积。
* 如果执行的是第二条分支，那么直接把旧的**threshold**值赋给新容量。但为何没调整**threshold**的值呢？请看R2部分的分析。

对于R2部分，作用是调整**threshold**的值，但为何单独抽离出来作为一个分支呢？因为在R1的第一条分支中，也有可能会出现这一部分，所以为了代码重用，单独抽离了出来。

如果新的容量和新的容量的扩容临界值都小于**MAXIMUM_CAPACITY**，那么就将**threshold**赋值为新容量的扩容临界值。否则将其赋值为**Integer.MAX_VALUE**。从这里也可以看出，**threshold**是可能比**MAXIMUM_CAPACITY**大的。

在**putVal**的X部分，是不会执行R3的。

继续回到**putVal**中，对于Y部分，如果当前键值对的键映射到的桶为空，那么直接填入。否则，（注意力放在Y部分的else中）：

* 如果键所映射到的桶的第一个元素(p)的键和新元素的键一致，那么暂存p到e。
* 如果p的类型为**TreeNode**，那么调用**putTreeVal**，因为此时结构以及变成树了。
* 如果前面两种情况都不是，那就遍历链表，在遍历过程中，有可能会出现第一种情况，也有可能顺利遍历到末尾，此时十分重要的一步到了，如果该链表的长度超过了**TREEIFY_THRESHOLD**（默认值是8，为什么是8，请看属性那一部分）的话，那么就执行**treeifyBin**操作。

最后需要对变量e进行处理，如果e不为**null**，说明已经存在键的值为e,key的映射。此时**onlyIfAbsent**的作用就体现出来了，如果其值为**false**或旧的值为**null**，那么就用新的值替换掉旧值，但这里并不能保证新的值就一定不是**null**。

然后执行**afterNodeAccess**方法，但是**HashMap**中的该方法为空，该方法的目的是：Callbacks to allow LinkedHashMap post-actions。然后返回旧的值。

至此，Y部分就分析完毕。

对于接下来的Z部分，只有当新插入的元素是链表的头节点或不存在已有映射(e==null)时，才会执行。

当目前映射的数量大于了**threshold**, 就调用**resize**方法扩容。该方法的源码上面已给出，并将其源码分成了几个部分，这里继续沿用。

R1部分的第一个分支一定会执行：

* 如果当前容量已经超过了最大容量(1<<30)，那么直接将扩容临界值设置为**Integer.MAX_VALUE**，直接返回**table**，表明已经无法扩容了。
* 否则，新容量为旧容量的两倍。并且当新容量仍然小于最大容量，且旧容量大于了默认的初始容量时，新的扩容临界值才是旧的扩容临界值的两倍。否则执行R2部分。

R2部分的分析与上面分析开辟初始空间时一致。总结一下，也就是说，当前容量大于了容量上限，那么就将扩容临界值设置为**Integer.MAX_VALUE**。否则，要么新临界值是旧临界值的两倍，要么就是新容量乘以负载因子。

R3部分是一定会执行的，也是主要的核心逻辑之处，主要实现键值对的转移：

循环遍历**table**，依次取出每个非空元素e，然后将其位置置为**null**：

* 如果e.next为空，那么直接对其进行哈希运算找下标，并填入到新的桶中。<font color="red">这里有一个疑问，就是之前在不同桶里面的单个元素，刚好都满足e.next==null而且在新数组容量下得到的映射下标又一样，会不会后面的把之前转移的就覆盖掉了？</font>

  后来发现我这个问题想复杂了，因为oldCap与newCap都是2的幂，展开成二进制会更容易想明白。

  上述问题可以转化为：用两个数表示数组长度，oldLen与newLen且newLen = 2*oldLen，元素p和q分别满足下列关系：

  p.hash % oldLen = i

  q.hash % oldLen = j

  i $\neq$ j （或p.hash$\neq$ q.hash) 且i, j $\in$[0, oldLen)$\bigcap$Z

  那么等式(p.hash % (oldLen\*2)) = (q.hash % (oldLen*2))是否有机会成立?

  证明：

  > 由已知条件可得：
  >
  > p.hash == a*oldLen + i
  > q.hash == b.oldLen + j
  >
  > 如果a,b都为偶数或都为奇数，那么一定不会成立
  > 如果a为偶数，b为奇数：
  > 	p.hash % (oldLen\*2) = i
  > 	q.hash % (oldLen\*2) = oldLen + j
  > 如果a为奇数，b为偶数：
  > 	p.hash % (oldLen\*2) = oldLen + i
  > 	q.hash % (oldLen\*2) = j
  >
  > 所以上述等式是绝对不可能成立的。

  注意，上面结论依赖于oldLen = newLen / 2，如果没有这样的关系，那么不一定能够保证上面等式一定不会成立：

  >假设oldLen=3, newLen=5, p.hash =4, q.hash=9
  >
  >当数组长度为3时，p映射后的下标为1， q被映射后的下标为0
  >
  >扩容后，长度为5时，p的映射为4，q的映射也为4。这就出现了冲突了。那么上面采用的直接赋值的操作就会出现p把q覆盖掉。

  <font color="red">所以说，这一步操作的可行性严格依赖于是按两倍来进行扩容。</font>

* 如果e是**TreeNode**类型的，那么调用其**split**方法处理。

* 否则，说明该元素后面跟着一条链表。

  起初，观察**e.hash & oldCap**半天，没看懂什么意思，后来恍然大悟，请时刻记住，不管是**oldCap**还是**newCap**，都是2的幂，二进制表示时只有其中一位为1，剩下31为全是0。所以这里的判断是为了筛选原位置不变和偏移量为**oldCap**的元素。拆分后的两条链表分别保存在以**loHead**和**hiHead**开始的两条链表中，前者保存的是位置不变的，后者保存的是偏移后的。

至此，**resize**方法就分析完毕。接下来，继续回到**putVal**方法中。

接下来执行**afterNodeInsertion**方法，但和**afterNodeAccess**一样，该方法在**HashMap**中为空，在**LinkedHashMap**中会用到。

现在把**put**方法的大体流程走完了。但是还有一些细节没有分析到，包括**newNode**，**putTreeVal**，**treeifyBin**,**split**方法没有分析到。

### newNode

该方法是**Node**内部类中的一员，比较简单，就是封装了new操作。

### putTreeVal

