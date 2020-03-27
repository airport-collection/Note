参考链接1：https://www.cnblogs.com/jingmoxukong/p/9408037.html



什么是IOC，官方原定义：

> It is a process whereby objects define their dependencies (that is, the other objects they work with) only through constructor arguments, arguments to a factory method, or properties that are set on the object instance after it is constructed or returned from a factory method. 

**org.springframework.beans**和**org.springframework.context**是IOC容器的基础。

**org.springframework.context.ApplicationContext**接口代表IOC容器。

ApplicationContext实现类：

* ClassPathXmlApplicationContext
* FileSystemXmlApplicationContext
* GenericApplicationContext



配置格式：

传统配置格式：XML

2.5版本开始，支持注解

3.0版本开始，支持Java代码



# XML配置中

顶级标签是==<beans/>==

通常一个逻辑层单独存在于一个xml文件中，可以在ClassPathXmlApplicationContext的构造方法中同时传递多个资源，也可以在其中一个xml文件中==<import/>==入其他xml文件，然后构造方法就传这一个就行。

可以通过bean的==id==或者是==name==属性命名，id只能有一个，name可以指定多个，通过逗号，分号或空格指定。id或name不是强制性要求指定的，如果没有指定，容器会为其生成一个唯一的名称，是类名首字母小写。

可以通过==<alias/>==在bean定义外部为其指定别名。

在xml中定义bean时，bean标签的class属性必须有，不同于id或name，class是强制性的（但有两种例外）。

实例化bean的方法

注意，IOC不一定只能管理标准bean，其他类型也可以进行管理。

* 构造器实例化
* 静态工厂方法
* 实例工厂方法

依赖注入有两种变种：

* 构造器注入
* setter&getter注入

当使用构造器注入的时候，如果参数类型是基本类型，必须在配置文件中指定类型，否则注入失败。如果不指定类型，也可以指定参数顺序下标（从0开始），这种方式开可以解决有类型重复的情况。还可以通过构造器的参数名称注入，这种情况不会有二义性，但是这种方式需要在编译时为项目添加debug标志，如果不添加此标志，也可以使用注解**@ConstructorProperties**({"years", "ultimateAnswer"})（e.g.）。

当使用setters注入的时候，同时也可以混用构造器注入。推荐对于强制性属性使用构造器注入，对于可选属性（有默认值）使用setters注入。构造器注入总是返回给调用者全实例化状态。setters注入的优点是在注入之后类容易修改。

单例bean或预实例化（默认情况下都是）bean在IOC容器启动时就被创建好了。其他情况，bean都是在被获取时才被创建。

# 循环依赖

A构造器注入B， B构造器注入A，这就循环了，会导致两个bean都不能被创建，而且会被IOC容器检测出来，并抛出==`BeanCurrentlyInCreationException`==异常。一种可行的解决方案是改用setters注入。如果没有循环依赖，比如A依赖B，那么IOC会先注入B，再注入A。

p命名空间

```xml
<bean id="myDataSource" class="org.apache.commons.dbcp.BasicDataSource"
      destroy-method="close"
      p:driverClassName="com.mysql.jdbc.Driver"
      p:url="jdbc:mysql://localhost:3306/mydb"
      p:username="root"
      p:password="masterkaoli"/>
```

通过配置一个==java.util.Properties==实例：

```xml
<bean id="mappings"
    class="org.springframework.context.support.PropertySourcesPlaceholderConfigurer">

    <!-- typed as a java.util.Properties -->
    <property name="properties">
        <value>
            jdbc.driver.className=com.mysql.jdbc.Driver
            jdbc.url=jdbc:mysql://localhost:3306/mydb
        </value>
    </property>
</bean>
```

IOC容器有hierarchy, 一个IOC容器可以有父容器。

==depends-on==属性不仅能控制创造bean的顺序，还能创造销毁的顺序。

当懒加载的bean是非懒加载单例bean的依赖时，还是会预初始化。

# 自动装配

优点：

* 能够有效减少注入时需要指定的属性或者是构造器参数。
* 自动装配可以随着对象的演化更新配置。

缺点：

* 显式的setters注入或构造器注入总是会覆盖自动装配。而且对于原始值及它们的数组，都不能使用自动装配。
* bean之间的依赖关系不会在文档中被明确体现出来，可能有失准确性。
* 对于依赖关系信息，不能使用自动工具将其生成文档。
* 容器中可能有多个匹配setters或构造器注入类型的bean,可能会产生二义性，从而导致抛出异常。

关于避免二义性，可以有以下几种方法：

* 完全不用自动装配
* 通过设置其==autowire-candidate==属性为==false==。但是该属性仅对基于类型的自动注入有影响，不会影响显式提供名称的注入，即使将该bean标记为不是自动装配的候选bean。还可以在容器一级（==default-autowire-candidates==属性），指定更加广泛的基于模式的匹配。
* 将bean的primary属性定义为==true==，以便将该bean设置为主要的bean。
* 通过注解配置更加精准的控制，参考后面基于注解的自动装配



模式：

* ==no==：（默认情况），一般不会自动注入依赖bean,需通过==ref==来指定
* ==byName==：根据属性名注入。spring会去容器中找和属性名一致的bean。
* ==byType==：根据类型注入。如果有多个bean都是这种类型的话，就会报错。如果没有这种类型的bean，那么什么事情都不会发生。
* ==constructor==：和==byType==一致，只不过这种自动装配的bean是提供给构造器作为参数用的。



# 方法注入

方法注入提出的目的是为了解决两个bean的声明周期不一样。比如单例的bean A依赖非单例的bean B，A只能创建一次，而B却不一样。如果B成为A的依赖，A内的B就无法重新创建。

## 查找方法注入

使用CGLIB字节码技术来实现。

## 任意方法替换



# Bean作用域

目前在版本5.2.4中一共有六种，其中四种仅用于支持web感知的ApplicaitionContext中：

* Singleton：默认值，在每个IOC容器中仅实例化一次。
* Prototype： 可以重复创建多次该bean。容器不会管理该作用域的bean的所有声明周期，比如销毁方法就不会自动调用。所以代码里面应该手动处理，以释放资源。
* request：每个http请求都有一个实例。
* session：每个http会话都有一个实例。
* application：作用域为ServletContext。
* websocket：整个WebSocket连接生命周期内有效。
* Spring3.0开始，支持线程作用域，但是默认没有注册，具体参考==SimpleThreadScope==api。
* 另外还可以自定义作用域。

# 生命周期回调函数

## 初始化

==org.springframework.beans.factory.InitializingBean==接口允许对bean注入依赖后执行初始化工作。但最好别直接让bean实现这个接口，因为会和spring框架本身耦合。最好使用注解==@PostConstruct==来指定bean初始化后需要执行的方法。

## 销毁

和初始化一样，最好别用==DisposableBean==接口。改用==@PreDestroy==注解。



以下事件前四种都是基于实现了ConfigurableApplicationContext接口的实例。





| 事件                       | 介绍                                                         |
| -------------------------- | :----------------------------------------------------------- |
| ContextRefreshedEvent      | 当ApplicationContext初始化或者刷新的时候发布。这里的初始化指的是所有的bean已经被加载，单例bean被创建，后置处理器被激活，应用上下文对象已可用。只要容器支持热刷新且没有被关闭，那么刷新事件就可以被触发多次。 |
| ContextStartedEvent        | 当ApplicationContext通过调用start()方法启动时发布。这里的启动是指所有的Lifecycle bean都接收到了明确的启动信号。 |
| ContextStoppedEvent        | 当ApplicationContext通过调用stop()方法终止时发布。这里的启动是指所有的Lifecycle bean都接收到了明确的终止信号。stop后还可以调用start重启。 |
| ContextClosedEvent         | 当ApplicationContext通过调用close()（后者是JVM shutdown hook）方法关闭的时候发布。这里的关闭是指所有的单例bean都已经被销毁，而且容器一旦关闭，就不能重启或刷新。 |
| RequestHandledEvent        | 当http请求已经完成时发布，表明http请求服务已经完成。         |
| ServletRequestHandledEvent | RequestHandledEvent的一个子类，增加了特定servlet的信息。     |

