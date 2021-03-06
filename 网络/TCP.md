# 特性

- 面向连接的运输层协议。
- 每一条连接只能由两个端点，也就是说是端到端通信。
- 提供可靠的交互，数据无差错，不丢失，不重复，按序到达。
- 提供全双工通信，也就是说任何一方在任何时候都能发送数据。
- 面向字节流。

# 可靠传输的工作原理

## 停止等待协议

当接收方没收到或收到的是错误的数据报时，那么发送方超过一段时间没有接收到确认的话，那么就进行**超时重传**，是通过一个超时计时器来实现的，如果收到了确认信号，那么就取消该计时器。为了实现这一点，必须注意以下几点：

* 必须保留已发送分组的副本
* 分组和确认分组都必须进行编号
* 计时器的超时时间应当比平均往返时间更长一些。

接收方若收到重复的报文，则丢弃，发送方若收到迟到的确认则忽略。





## 连续ARQ协议

该协议规定，发送方每收到一个确认，就把窗口先前滑动一个分组的位置。接收方采用累计确认的方式，也就是说不必每收到一个就确认一次，可以累计收到多个分组后一次性的确认，这表示最后一个分组以前的都已经被正确地收到了。

该协议的缺点是，不能向发送方反映接收方以及正确收到的所有分组信息。也就是说，如果发送方发送了5个分组，但第3个分组丢失了，就只能把后面三个分组再发送一次，这就是回退N步协议（GO-BACK-N）。

# 头部字段

![](https://img-blog.csdnimg.cn/20190902195359730.png?)

TCP报文的首部前20个字节是固定的，后面4n(n是整数)字节是可选的。下面是一些重要字段：

* 源端口和目的端口：各占两个字节
* 序号(seq)：占4字节，序号使用mod2^32^运算。这里的序号指的是字节的序号，而不是报文段的序号。
* 确认号(ack)：占4字节，确认号指的是接收方希望发送方发送的下一个报文段的第一个字节序列的序号。若确认号为N，则说明N-1为止的字节都已经被正确收到。
* 数据偏移：该字段描述TCP报文段中数据的起始处距离报文的起始处的距离，数据的前面是头部，也就是说该字段描述的是首部的长度，固定部分为20字节，但是还有可选部分。该字段长度为4位，能够表示的最大十进制数字为15，但是单位是字（长度为4字节），所以也就决定了长度最长为60字节，可选部分最长为40字节。

下面是几个控制位：

* ACK：当ACK=1时，代表确认号有效，当为0时，则代表无效。在连接建立后的所有报文中，都必须把ACK置为1。
* SYN：在建立连接时，用来同步序号。当ACK=0，SYN=1，则代表这是一个连接请求报文。若同意连接，则应该在相应报文段中设置ACK=1,SYN=1。SYN为1的报文段不能携带数据，但要消耗掉一个序列号。
* FIN：



* 窗口：这里的窗口值指的是发送本报文段的一方的接收窗口。该值告诉对方，从本报文段中的确认号算起，还可以有多少字节的缓存空间。该值明确指出了对方现在能够发送的最大字节数。



# 可靠传输的实现

## 滑动窗口

滑动窗口的单位是字节，不是报文段。

发送方的滑动窗口内保存了**已发送但尚未收到确认**的报文段，以及应用程序**准备发送但尚未发送**的报文段。发送缓存的后沿与发送窗口的后沿是重合的。

 接收方的滑动窗口内保存的是正确接受了但还未被应用程序读取的报文段以及未正确收到的报文段。

对于未按时到达的报文段，TCP协议并没有明确规定。目前有两种办法，一是丢掉，类似回退N步协议，但很明显，这种方法对网络利用率不高。而是暂存，待收到连续的报文段后，再向上层提交。

## 超时重传时间的选择

发送方发送报文后，如果一段时间内都没有收到确认，那么就要重传。但是这个时间值却不是很好确定，如果太短，会加剧网络的负载，如果太长，会导致接收端对应用程序相应迟缓从而降低传输效率。

TCP采用了一种自适应的算法。一个报文段从发出直到收到确认为止，这段时间叫做往返时间(RTT)。TCP保留了一个加权往返平均时间（RTT~S~）。

新的RTT~s~ = (1-$\alpha$) * 旧的RTT~s~ + $\alpha$ * 新的RTT​  ，其中  0 $\leq \alpha \leq$ 1

超时计时器设置的超时重传时间RTO应该略大于上面的RTT~s~。具体是多少不在列出。

## 选择重传

上面说到，接收端通常对于没有按需到达的报文段采用先暂存的方法，那么以前那种确认机制就不行，否则发送端还是会发送多个（包括已被缓存的）报文段。选择确认就是一种修正方法。如果要使用这种方法，那么在建立TCP连接的时候就应该在TCP的首部添加“允许SACK”的选项。要重传哪些是在TCP首部的自定义的部分定义的，但是最多只能添加40个字节的自定义字段，所以这种方法不是很常用。

# 流量控制

流量控制是让发送方不要发送太快，让接收方来得及接收。

# 拥塞控制

拥塞控制是指避免过多的数据注入到网络中，这样可以使网络中的路由器或链路不致于过载。拥塞控制是为了整个网络而考虑的，是一个全局性的过程。而流量控制是指点对点的通信量的控制。

控制方法：

慢开始和拥塞避免

慢开始起初窗口滑动得很慢，然后后面一次是前一次的两倍，直到拥塞避免阈值，然后采用单个数量的增长，如果出现超时，则又回到慢开始，同时降低阈值。

快重传与快恢复





主动队列管理AQM



# 建立连接



所谓三次握手(Three-way Handshake)，是指建立一个 TCP 连接时，需要客户端和服务器总共发送3个包。

三次握手的目的是连接服务器指定端口，建立 TCP 连接，并同步连接双方的序列号和确认号，交换 TCP 窗口大小信息。在 socket 编程中，客户端执行 `connect()` 时。将触发三次握手。

- 第一次握手(SYN=1, seq=x):

  客户端发送一个 TCP 的 SYN 标志位置1的包，指明客户端打算连接的服务器的端口，以及初始序号 X,保存在包头的序列号(Sequence Number)字段里。

  发送完毕后，客户端进入 `SYN_SEND` 状态。

- 第二次握手(SYN=1, ACK=1, seq=y, ACKnum=x+1):

  服务器发回确认包(ACK)应答。即 SYN 标志位和 ACK 标志位均为1。服务器端选择自己 ISN 序列号，放到 Seq 域里，同时将确认序号(Acknowledgement Number)设置为客户的 ISN 加1，即X+1。 发送完毕后，服务器端进入 `SYN_RCVD` 状态。

- 第三次握手(ACK=1，ACKnum=y+1)

  客户端再次发送确认包(ACK)，SYN 标志位为0，ACK 标志位为1，并且把服务器发来 ACK 的序号字段+1，放在确定字段中发送给对方，并且在数据段放写ISN的+1

  发送完毕后，客户端进入 `ESTABLISHED` 状态，当服务器端接收到这个包时，也进入 `ESTABLISHED` 状态，TCP 握手结束。

三次握手的过程的示意图如下：

![three-way-handshake](https://raw.githubusercontent.com/HIT-Alibaba/interview/master/img/tcp-connection-made-three-way-handshake.png)

为什么连接的时候是三次握手？

第一次握手：客户端发送网络包，服务端收到了。这样服务端就能得出结论：客户端的发送能力、服务端的接收能力是正常的。

第二次握手：服务端发包，客户端收到了。这样客户端就能得出结论：服务端的接收、发送能力，客户端的接收、发送能力是正常的。不过此时服务器并不能确认客户端的接收能力是否正常。

第三次握手：客户端发包，服务端收到了。这样服务端就能得出结论：客户端的接收、发送能力正常，服务器自己的发送、接收能力也正常。

能不能只使用两次握手？

如果只使用两次握手，那么客户端发起了一次请求后，超时，又重传一次，服务端收到相应确认，客户端接受端确认建立连接，发送数据。数据发送完毕，连接关闭，服务端收到了之前超时的请求，就又回应了一次客户端，但是此时客户端不会理会服务端发送来的确认，导致了服务端的浪费。



# 关闭连接

建立一个连接需要三次握手，而终止一个连接要经过四次挥手。

1. 首先客户端A向服务端B发送FIN=1的终止请求报文，A进入FIN-WAIT-1状态，B收到后响应确认，B进入CLOSE-WAIT状态

2. A收到确认后，进入FIN-WAIT-2状态，此时不能直接关闭，可能B还没有接受完A发送的数据，

3. 待B接收完后再次发送确认，B发送了第二次确认后进入LAST-ACK状态
4. A收到B的第二次确认后，发送第四个，也是本次连接的最后一个报文，A发送完后进入2MSL期间，B收到后，关闭连接。A等2MSL期间结束后关闭连接。

![](https://raw.githubusercontent.com/HIT-Alibaba/interview/master/img/tcp-connection-closed-four-way-handshake.png)