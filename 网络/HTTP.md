



# 请求头

![](https://pic3.zhimg.com/80/v2-c920e11f00c2510d9a8fa4f4009d0f2e_720w.jpg)

## 请求方法

* GET: GET方法请求一个指定资源的表示形式. 使用GET的请求应该只被用于获取数据。
* HEAD: HEAD方法请求一个与GET请求的响应相同的响应，但没有响应体。
* POST: POST方法用于将实体提交到指定的资源，通常导致在服务器上的状态变化或副作用。
* PUT: PUT方法用请求有效载荷替换目标资源的所有当前表示。
* DELETE: DELETE方法删除指定的资源。
* CONNECT: CONNECT方法建立一个到由目标资源标识的服务器的隧道。
* OPTIONS: OPTIONS方法用于描述目标资源的通信选项。
* TRACE: TRACE方法沿着到目标资源的路径执行一个消息环回测试。
* PATCH: PATCH方法用于对资源应用部分修改。在HTTP1.1之后引入看，详情请见RFC5789。

上面各种方法可能是在不同的版本中引入的。

<font color="red">GET方法与POST到底有什么不同？</font>

HTTP协议本身并没有url的长度进行限制，。但是服务器和浏览器都会对url的长度有所限制（大多数浏览器通常都会限制url长度在2K个字节，而且最多处理64K大小的url。超过的部分，恕不处理）。**GET**方法也可以有请求体，也可以把数据放在请求体中，但是不能保证服务器一定会处理，甚至有的浏览器会直接忽略。**POST**方法按理说也可以把参数放在URL上，但是最好别这样做，最好符合各自的语义。

**GET**产生一个TCP数据包；**POST**产生两个TCP数据包(但不是绝对的)。对于**GET**方式的请求，浏览器会把http header和data一并发送出去，而对于**POST**，浏览器先发送header，服务器响应100 continue，浏览器再发送data，但也不是一定的，Firefox（Chrome也一样）就只发送一次。

另外，对于**GET**方法，在约定中，我们的参数是写在 `?` 后面，用 `&` 分割。但是也可以自己定义，只要服务器能够解析，例如一种较为流行的写法：http://www.example.com/user/name/chengqm/age/22。

**POST**并不比**GET**安全，因为HTTP是明文传输，都不安全，要安全就使用HTTPS。

总结如下，**GET**和**POST**从底层来说，并没有区别，但是规范为其定义的语义有很大的区别，在开发中应该合理遵循各自的语义。

**PUT**与**PATCH**的区别：

+ **PATCH**表示一般用于部分更新，而**PUT**用于整体更新。

* 当资源不存在时，**PATCH**会创建一个新的资源，而**PUT**只会对已在资源进行更新。

## 常见的头部字段：

Accept*

```tet
Accept
Accept-Charset
Accept-Datetime	
Accept-Encoding
Accept-Language
Access-Control-Request-Method,
Access-Control-Request-Headers
```

**Accept**代表了可接受的响应的媒体类型（MIME Type），**Accept-Charset**代表了可接受的字符集。**Accept**里面也可以设置字符集，但是强烈建议不这么做。**Accept-Encoding**表示的是可接受的页面编码，这个编码与字符编码（Accept-Charset）不是一个该概念。这里的charset指的是内容本身的编码，encoding指的是将内容压缩采用的编码方式，如gzip。对于很多**Accept\***字段，可以设置一个属性为**q**，表示**relative quality factor**,取值为0~1，表示用户更喜欢哪种选择。因为预检请求所使用的方法总是**OPTIONS**,所以**Access-Control-Request-Method**表示实际的方法类型。请求头 ***Access-Control-Request-Headers **出现于 [preflight request](https://developer.mozilla.org/zh-CN/docs/Glossary/Preflight_request) （预检请求）中，用于通知服务器在真正的请求中会采用哪些请求头。

**Authorization**: 设置HTTP身份验证的凭证。

**Cache-Control** 设置请求响应链上所有的缓存机制必须遵守的指令。

**Content-Length** 设置请求体的字节长度。

**Content-MD5** 设置基于MD5算法对请求体内容进行Base64二进制编码。

**Content-Type** 设置请求体的MIME类型（适用POST和PUT请求)。

**Cookie** 设置服务器使用Set-Cookie发送的http cookie。

**Forwarded** 披露客户端通过http代理连接web服务的源信息。



# 响应头





# 参考

https://juejin.im/post/5e76bd516fb9a07cce750746