# 介绍
接口Environment描述了应用的profile和properties。
## 什么是Profile
A profile is a named, logical group of bean definitions to be registered with the container 
only if the given profile is active.
## 什么是Properties
Properties play an important role in almost all applications and may originate from a variety of sources: 
properties files, JVM system properties, system environment variables, JNDI, servlet context parameters, 
ad-hoc Properties objects, Map objects, and so on.

# @Profile
该注解作用在类后者方法上，比如常用的就是指定开发环境或生产环境：
@Profile("development")
@Profile("production")
因为开发时和生产时用的数据库等资源都不一致，所以为了方便分别指定两套环境。
环境信息还可以运算，有三种，逻辑分别对应Java中的逻辑与、逻辑或、逻辑非。
比如：@Profile("production & (us-east | eu-central)")
为了方便，还可以声明一个元注解，即：
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Profile("production")
public @interface Production {
}
```
以后就可以直接使用@Production了。

如果一个@Configuration修饰的类同时被@Profile修饰了，那么所有的bean定义和import操作都将会受到相应限制；
比如一个类被如下修饰：@Profile({"p1", "p2"})
那么只有在p1或p2被激活的时候才有效。
当@Profile和@Bean共同修饰一个方法时，对于重载方法（包括构造器），所有的@Profile条件需要一致；
如果不一致，则以第一个声明的为准。因此，@Profile注解不能用来选择要给带有特定参数的重载方法。

## 激活Profile
ctx.getEnvironment().setActiveProfiles("development");
其中ctx就是上下文对象。还可以什么多种：
ctx.getEnvironment().setActiveProfiles("profile1", "profile2");
还可以使用系统环境变量指定：
-Dspring.profiles.active="profile1,profile2"

@Profile("default")指定默认的profile。

# PropertySource简介

```java
ApplicationContext ctx = new GenericApplicationContext();
Environment env = ctx.getEnvironment();
boolean containsMyProperty = env.containsProperty("my-property");
System.out.println("Does my environment contain the 'my-property' property? " + containsMyProperty);
```
这个代码段示例了向IOC容器询问是否存在一个名为“my-property”的属性，IOC的环境对象Environment内保存有PropertySource对象，
通过对其进行查询就可得知结果。
StandardEnvironment配置了两种PropertySource，一种代表JVM系统属性（System.getProperties()），
另一种代表系统环境变量（System.getenv()）。

    补充：虚拟机参数（VM options）与环境变量(environment variables)和程序参数（Program Arguments）的区别：
    程序参数通常用来传递给main方法的args的。
    虚拟机参数被传递给Java解释器，然后作为系统属性。
    传入虚拟机参数： -Dspring.profiles.active="profile1,profile2"，然后通过System.getProperties得到如下键值对：
    key=spring.profiles.active 
    value=profile1,profile2
    系统环境变量被临时添加到物理机系统中去，运行时可以通过System.getenv获得。
   
对于下面配置：
vm options: -Dspring.profiles.active="profile1"
environment variable: spring.profiles.active=profile2

那么context.getEnvironment().getProperty("spring.profiles.active")得到的是profile1，而不是profile2。
特别注意两者的处理方式不同。
对于Spring IOC Environment来说，优先级是虚拟机参数大于环境变量，相当于先找小范围再找大范围。

通过执行 ctx.getEnvironment().getPropertySources().addFirst(new MyPropertySource());
可以添加属性,并将其放在属性列表的最前面（addFirst）。

# 使用@Properties注解
该注解只能用在类上，可重复声明（Java8支持）。
通过@PropertySource("/resource/database.properties")注解可以向环境对象注入属性文件中的所有属性
同时路径中还可以指定已注册的属性，比如在虚拟机参数后者是环境变量中添加my.placeholder，下面的代码等价于上面的代码。
@PropertySource("/${my.placeholder:resource}/database.properties")
如果my.placeholder没有指定，那么默认使用resource来替换占位符,注意字符串resource需要放在${}内部。


