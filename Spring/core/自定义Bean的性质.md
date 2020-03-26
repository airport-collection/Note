# 声明周期回调
JSR-250中的注解@PostConstruct与@PreDestroy被认为是生命周期回调的最佳实践，
相比实现InitializingBean与DisposableBean而言，那两个注解能够避免和Spring耦合。
在内部，是依靠BeanPostProcessor来实现生命周期的调用的。
## 绑定生命周期回调的方式
* 通过InitializingBean与DisposableBean接口
* 通过@PostConstruct与@PreDestroy注解
* 自定义init()与destroy()方法
如果同时定义了以上三种，那么执行顺序如下：

对于初始化：
1. Methods annotated with @PostConstruct
2. afterPropertiesSet() as defined by the InitializingBean callback interface
3. A custom configured init() method

对于销毁：
1. Methods annotated with @PreDestroy
2. destroy() as defined by the DisposableBean callback interface
3. A custom configured destroy() method

但是上面三种方式与BeanPostProcessor不同之处在于，
上面的方式都是针对于某个特定bean来配置的，但是另一种类似一种通用实现，
可以为多个不同的bean指定相同的回调。

任何Spring管理的Bean类型可以实现Lifecycle接口,


# ApplicationContextAware & BeanNameAware
**Aware** 这个词在Spring中是经常出现的，意思为感知，
比如ApplicationContextAware就是让bean感知到创建他们的容器，
从而让bean对其容器进行操作；BeanNameAware就是让bean获取在配置bean时，
赋予的ID。

# 其他Aware接口

