Set接口是Collection接口的三大子接口（List, Queue, Set）之一，这类集合中不能有重复元素，也最多只能有一个**null**值（但具体还应该看实现类的要求），这里说的重复元素指的是两个元素a,b且a.equals(b)为true。需要特别注意如何处理集合中的可变对象，因为这些可变对象可能影响equals的比较。Set中不能把自身作为元素存放在其中。



## HashSet

其依赖于**HashMap**来实现的。



## TreeSet



