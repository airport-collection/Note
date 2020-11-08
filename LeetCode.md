# 总计

共 <font style="color:deeppink;font-size:20px;margin:20px">2</font> 题

<font color="#55efc4">简单</font>

<font color="#fdcb6e">中等</font>

<font color="#d63031">困难</font>

# 2020-09-21



## [977. 有序数组的平方](https://leetcode-cn.com/problems/squares-of-a-sorted-array/) 	<font color="#55efc4">简单</font>

本题有两种解法，排序和双指针。采用排序法AC, 但是双指针法更优。

## [面试题17.10. 主要元素](https://leetcode-cn.com/problems/find-majority-element-lcci/)	<font color="#55efc4">简单</font>

本题采用了排序法AC, 另外还有哈希表和<font color="deeppink">摩尔投票</font>（更优）。



# 2020-09-22

## [1588. 所有奇数长度子数组的和](https://leetcode-cn.com/problems/sum-of-all-odd-length-subarrays/)	<font color="#55efc4">简单</font>

本题采用了一个O(n^3^)的解法，目前来说没有找到更优的解法，可能也由于本地较新，所以做的人很少。

## [628. 三个数的最大乘积](https://leetcode-cn.com/problems/maximum-product-of-three-numbers/)	<font color="#55efc4">简单</font> :heavy_multiplication_x:

本题采用暴力解法，结果超出时间限制。

参考官方解法，得知最大的积要么是最大的三个数之积，要么是最小的两个数与最大的那个数之积。

目的是找出上面所说的那几个数，然后比较结果。



## [509. 斐波那契数](https://leetcode-cn.com/problems/fibonacci-number/)	<font color="#55efc4">简单</font>

本题利用快速幂来解决，类似的题目还有求斐波那契数列的第N项对于某个数取余的结果。

下面采用矩阵快速幂（优化后的矩阵求幂）的方法，规定第一项为0，如果以后遇到第一项为1的情况，那么则先对项数执行减1操作。

斐波那契数组有下列关系成立：


$$
\begin{bmatrix}
   1 & 1 \\
   1 & 0
\end{bmatrix}
^n
\begin{bmatrix}
   F_1 \\
   F_0
\end{bmatrix}
=
\begin{bmatrix}
   F_{n+1} \\
   F_n
\end{bmatrix}
$$
那么如果要求 $F_n$ ， 则可以把问题转化为求这个特殊矩阵的幂。这里可以优化一下，如果n=9，那么大可不必从头开始计算，因为：
$$
\begin{bmatrix}
   1 & 1 \\
   1 & 0
\end{bmatrix}
^8
=
\begin{bmatrix}
   1 & 1 \\
   1 & 0
\end{bmatrix}
^4
\begin{bmatrix}
   1 & 1 \\
   1 & 0
\end{bmatrix}
^4
$$

$$
\begin{bmatrix}
   1 & 1 \\
   1 & 0
\end{bmatrix}
^4
=
\begin{bmatrix}
   1 & 1 \\
   1 & 0
\end{bmatrix}
^2
\begin{bmatrix}
   1 & 1 \\
   1 & 0
\end{bmatrix}
^2
$$

$$
\begin{bmatrix}
   1 & 1 \\
   1 & 0
\end{bmatrix}
^2
=
\begin{bmatrix}
   1 & 1 \\
   1 & 0
\end{bmatrix}
\begin{bmatrix}
   1 & 1 \\
   1 & 0
\end{bmatrix}
$$



对于剩下的幂，再利用普通方法计算:


$$
\begin{bmatrix}
   1 & 1 \\
   1 & 0
\end{bmatrix}
^9
=
\begin{bmatrix}
   1 & 1 \\
   1 & 0
\end{bmatrix}
^8
\begin{bmatrix}
   1 & 1 \\
   1 & 0
\end{bmatrix}
$$

另外，关于n的取法问题：$\begin{bmatrix}
   F_{n+1} \\
   F_n
\end{bmatrix}$ or $\begin{bmatrix}
   F_{n} \\
   F_{n-1}
\end{bmatrix}$ ，矩阵分别应求$n(n \ge 0)$ 或 $n-1(n \geq 1)$次幂。本题采用的是前者。

对于求余的扩展问题，其实可以直接在矩阵求幂的过程中就取余，否则大部分情况下都会溢出。



## [剑指 Offer 04. 二维数组中的查找](https://leetcode-cn.com/problems/er-wei-shu-zu-zhong-de-cha-zhao-lcof/)	<font color="#55efc4">简单</font>

本题采用暴力解法AC, 但是根据评论区发现与二叉树有关， 可以把最左上角看作二叉树的根节点，往下走看作是左子节点，右边是右子节点。

另外，本题采用Golang，注意判错。虽然参数为\[][]int, 但是[]也可以传入，此时len(matrix) == 0 && matrix[0] == nil。



## [747. 至少是其他数字两倍的最大数](https://leetcode-cn.com/problems/largest-number-at-least-twice-of-others/)	<font color="#55efc4">简单</font>	:heavy_multiplication_x:

本题其实可以直接请求数组中最大和次大的数，然后直接比较两者就可以了。此题居然提交了8次才AC，确实不应该，卡在了如何在一次遍历中找出最大和次最大的数。另外题中隐含了一个条件，就是数组中肯定含有非负数。

本题延伸出来的问题，如何一次遍历中找出数组中最大的那两个数？





# 2020-09-23

## [1266. 访问所有点的最小时间](https://leetcode-cn.com/problems/minimum-time-visiting-all-points/)	<font color="#55efc4">简单</font>

一次AC，注意在坐标轴上分情况讨论。如果两个点的x或y坐标相同，那么直接计算其差值，如果都不相同，那么取最大的差值。

 

## [1260. 二维网格迁移](https://leetcode-cn.com/problems/shift-2d-grid/)	<font color="#55efc4">简单</font>	:heavy_multiplication_x: :six_pointed_star:	

注意此题不是交换元素，而是移动。首先转化问题，将二维数组转化为一维，然后在数组内循环移动。特别注意，移动次数可能超过整个一维数组的长度，此时可以做对长度取余的操作。将数组最后k个元素取出来放在剩下元素的最前面，然后再转回二维数组。

该题的问题转化非常重要，也很典型，应该引起重视。



# 2020-09-24

## [1160. 拼写单词](https://leetcode-cn.com/problems/find-words-that-can-be-formed-by-characters/)	<font color="#55efc4">简单</font>

此题首先根据chars生成map, 然后遍历字符串数组，再对每个字符串遍历，判断每个字母在map中的情况决定是否符合要求。但是这种方法效率不是太高。



## [1331. 数组序号转换](https://leetcode-cn.com/problems/rank-transform-of-an-array/)	<font color="#55efc4">简单</font>

本题也是采用数组和map结合的方法，一次AC。该题是典型的对数组排序问题，注意对于重复元素的特殊性要求。



## [1534. 统计好三元组](https://leetcode-cn.com/problems/count-good-triplets/)	<font color="#55efc4">简单</font>

本题虽然一次AC，但是采用的是暴力法，效率不高，如果严格要求或者是数据量更大一些，这种解法还不能够AC。后续可以从<font color="deeppink">离散化或树状数组</font>的角度优化。



# 2020-09-25

## [88. 合并两个有序数组](https://leetcode-cn.com/problems/merge-sorted-array/)	<font color="#55efc4">简单</font>

本题采用的是合并排序法，虽然一次AC，但是效率很低下。而且在审题时没注意两个数组已经有序了。可以采用<font color="deeppink">双指针法</font>，每次读取时判断两个指针指向的元素谁小，就把它存入结果数组中。但是这种方法还不是最优的，因为使用了额外的空间，可以采用双指针从后向前遍历。



# 2020-09-27

## [面试题 02.02. 返回倒数第 k 个节点](https://leetcode-cn.com/problems/kth-node-from-end-of-list-lcci/)	<font color="#55efc4">简单</font>

本题是经典的面试题了，采用<font color="deeppink">双指针法</font>一次AC。



# 2020-10-16

## **[110. 平衡二叉树](https://leetcode-cn.com/problems/balanced-binary-tree/)** <font color="#55efc4">简单</font>