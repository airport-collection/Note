# 几种容器构造的方式
1. 简单构造
将bean的class传入AnnotationConfigApplicationContext的构造器，得到的上下文对象就包含这些bean

2. 通过注册
构造时不指定任何class, 事后通过上下文的register方法注入

3. 扫描
有两种方法，一种是在configuration类上加@ComponentScan注解，第二种是通过上下文对象扫描

注解AnnotationConfigApplicationContext于Spring 3.0被引进.
通过AnnotationConfigWebApplicationContext可以获得基于web的context对象

# @Bean注解的使用
## 声明bean
可以将该注解用在方法上，那么该bean的类型为返回值类型，这个返回值类型可以是实际类型实现的接口类型或者是父类类型。
默认情况下bean的名称就是方法名。

## bean依赖    
方法可以有参数，这些参数通常作为bean的依赖。方法参数的名称就是bean的名称。

## 生命周期回调
@Bean注解除了支持一般的生命周期回调，还支持JSR-250中的注解@PostConstruct和@PreDestroy
如果bean实现了接口：InitializingBean, DisposableBean，相应的方法会被调用。
另外@Bean注解还有参数可以指定相应的方法可以执行：
    @Bean(initMethod = "init")
    @Bean(destroyMethod = "cleanup")
其中init和cleanup方法分别是bean中定义的方法
通常Java配置的Bean都会有公共的“close“方法，如果不想调用默认的，就这样指定：@Bean(destroyMethod="")

可以通过注解@Scope("prototype")为bean指定作用域，默认的作用域是单例的。
还可以指定bean的别名：@Bean({"dataSource", "subsystemA-dataSource", "subsystemB-dataSource"})
以及bean的描述：@Description("Provides a basic example of a bean")

# @Configuration注解的使用
该注解是类级别的
## 注入内部依赖
Spring文档演示的是一个方法内调用另一个方法的返回值作为参数

# 完善基于Java的配置
## 使用@Import注解
就像在XML中使用<import/>标签一样，该注解也是引入其他的@Configuration配置。如：@Import(ConfigA.class)，作用在类上。
Spring4.2的时候，@Import注解允许引入一般的@Component类，和AnnotationConfigApplicationContext.register一样。
@Configuration中构造器注入只有4.3及以上版本支持。而且bean（假如A注入到B中，那么这里指的是B）中只有一个构造器，
那么不需要指定@Autowired。

## 和XMl配置混用




