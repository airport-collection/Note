# AOP中的概念

![](https://www.codejava.net/images/articles/frameworks/spring/Understanding%20Spring%20AOP/CrossCuttingConcersn.png)

## 基本概念

* Aspect(切面)：在应用程序中所关心的问题，比如日志，事务管理，安全验证。
* Join point(连接点)：在程序执行过程中的一点，比如方法执行或异常抛出；在Spring中，只指方法执行。
* Advice(通知)：是切面的具体实现，切面比较抽象，只是一种概念而已。通知是在具体连接点采取的动作。可以分为前置、后置、环绕三种通知。
* Pointcut(切入点)：切入点指出哪些连接点需要应用切面，通俗一点说就是哪些方法需要应用哪种通知。
* Introduction(引入)：向已有类中的添加字段或者是方法。
* Target object(目标)：在一个或多个切面上应用通知的对象，也就是包含业务逻辑主体的对象。
* AOP proxy(代理)：目标对象的代理对象，在SpringAOP中，使用JDK动态代理或者是CGLIB动态代理。
* Weaving(织入)：把切面应用到目标对象从而创建代理对象的过程。根据代理的不同，CGLIB是编译时织入，Spring AOP等纯Java AOP框架是运行时织入。

以上概念比较抽象，前面四个概念，切面和连接点像是抽象级别的东西，而通知和切入点就是比较实际的东西，分别是切面和连接点的实现。

## 通知类型

* 前置通知：在连接点之前执行，但是没有阻止执行的能力i，除非抛出异常。
* 正常返回通知：
* 异常返回通知：
* 后置通知：不论连接点是否正常退出，也就是不管是正常通知还是异常通知，都会执行。
* 环绕通知：在方法调用前后都会执行相应的通知。还可以根据其返回值或是否抛出异常来选择是否继续执行相应的方法。

Spring AOP默认使用JDK代理，当然也可以使用CGLIB代理，两者的区别在于JDK代理针对实现了实现了接口的对象，而CGLIB是根据继承来代理的。当bean实现了接口，那么spring会采用JDK代理，否则使用的是CGLIB代理。当然，也可以强行指定哪种。

## AspectJ

在Spring中使用AspectJ需要先导入包，然后再开启支持。

注解声明中，为了让Spring自动探测，@Aspect注解是不够的，还需要加上@Component注解。在Spring AOP中，切面不能被其他切面所通知。

切入点由两部分组成，一是切入点签名，二十切入点表达式。下面这个例子有助于区分这两者：

```java
@Pointcut("execution(* transfer(..))") // the pointcut expression
private void anyOldTransfer() {} // the pointcut signature
```

所支持的标识符

* execution: 用于匹配方法执行
* within: 限制连接点的类型
* this: 限制bean为给定类型
* target: 限制目标对象为给定类型
* args: 限制参数为给定类型
* @target: 限制执行对象的类被给定类型的注解修饰
* @args: 限制运行时参数类型被指定注解所修饰
* @within: 限制连接点（方法）所在类的注解类型中存在给定的类型。
* @annotation: 限制连接点（方法）被给定注解所修饰。

截至Spring v5.2.4, Spring所支持的标识符就有以上这几个，未来可能还会增加。AspectJ中还有其他的标识符，对于Spring中不支持的，Spring会抛出==IllegalArgumentException==。

上面target和this很容易弄混，注意区分。（暂时不知道区别）

可以使用Java语言中的逻辑运算符来绑定多个标识符。

对于excution标识符，其结构如下：

```java
execution(modifiers-pattern? ret-type-pattern declaring-type-pattern?name-pattern(param-pattern)
          throws-pattern?)
```

除了ret-type-pattern，name-pattern，param-pattern之外都是可选的。

通知表达式中绑定的参数是无法通过反射获取的，所以Spring AOP采用如下策略来确定参数名称：

* 如果该名称被明确指出，那么该名称会被采用。任何通知注解都有一个可选属性：**==argNames==**。
* 如果上面所述的那个属性没有指定的话，那么回去调试信息的本地变量表中去找相应的参数。
* 如果调试信息里面没找到，那么Spring会自动推断，如果出现模棱两可的情况，那么会抛出==AmbiguousBindingException==异常。
* 如果还没找到，那么会抛出==IllegalArgumentException==异常。

简单AOP的配置：

```xml
<!-- 通知的实例 -->
<bean id="advice" class="com.isoftstone.util.Advice" />

<aop:config>
    <aop:advisor advice-ref="txAdvice" pointcut-ref="pointCut" />
    <aop:aspect ref="advice">
        <aop:pointcut
                      expression="execution(* com.isoftstone.service.impl..*(..))"
                      id="pointCut"/>
        <aop:before method="before" pointcut-ref="pointCut" />
        <aop:after method="after" pointcut-ref="pointCut" />
        <aop:around method="around" pointcut-ref="pointCut" />
        <aop:after-throwing method="exception" pointcut-ref="pointCut" />
    </aop:aspect>
</aop:config>

<!-- 进行事务规则的定义(事务增强通知) -->
<tx:advice id="txAdvice">
    <tx:attributes>
        <!-- 添加/删除/修改 -->
        <tx:method name="insert*" propagation="REQUIRED" />
        <tx:method name="add*" propagation="REQUIRED" />
        <tx:method name="save*" propagation="REQUIRED" />
        <tx:method name="update*" propagation="REQUIRED" />
        <tx:method name="edit*" propagation="REQUIRED" />
        <tx:method name="delete*" propagation="REQUIRED" />
        <!-- 查询 -->
        <tx:method name="select*" propagation="NOT_SUPPORTED"
                   read-only="true" />
        <tx:method name="get*" propagation="NOT_SUPPORTED"
                   read-only="true" />
        <tx:method name="find*" propagation="NOT_SUPPORTED"
                   read-only="true" />
    </tx:attributes>
</tx:advice>
```

上面Advice的代码：

```java
public class Advice {
	public void before() {
		System.out.println("before......");
	}
	public void after() {
		System.out.println("after.......");
	}
	public Object around(ProceedingJoinPoint pjp) throws Throwable {
		System.out.println("around before........");
		Object obj = pjp.proceed();
		System.out.println("arund after........");
		return obj;
	}
	public void exception() {
			System.out.println("exception......");
	}
}
```









