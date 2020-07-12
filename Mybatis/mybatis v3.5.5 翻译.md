# Introduction

## introduction

### what is mybatis

MyBatis是一个优秀的持久化框架，支持自定义SQL，存储过程以及高级映射。MyBatis消除了几乎所有在原生JDBC中手动设置参数和取结果的操作。MyBatis可以通过简单的XML或注解进行配置，还能够将基本类型，Map接口以及Java的pojo对象映射为数据库记录。



# Getting Started

## Getting started

### Installation

为了使用MyBatis，你必须将**mybatis-x.x.x.jar**文件引入到类路径下。如果你使用Maven，那么可以将以下依赖加入到你的**pom.xml**文件中：

```xml
<dependency>
    <groupId>org.mybatis</groupId>
    <artifactId>mybatis</artifactId>
    <version>x.x.x</version>
</dependency>
```

### Building SqlSessionFactory from XML

每个MyBatis应用都以SqlSessionFactory实例为中心。一个SqlSessionFactory实例可以通过SqlSessionFactoryBuilder来获得。SqlSessionFactoryBuilder能够从一个XML配置文件，或者从一个配置类的实例中构建一个SqlSessionFactory实例。

从一个XML文件中构建SqlSessionFactory实例是非常简单的。推荐使用该配置文件放置在类路径下，但是也可以使用任何使用文件路径或URL指向的文件。MyBatis包含了一个工具类，叫做**Resources**, 包含了大量的方法，这些方法使从类路径下或其他地方加载资源更容易。

```java
String resource = "org/mybatis/example/mybatis-config.xml";
InputStream inputStream = Resources.getResourceAsStream(resource);
SqlSessionFactory sqlSessionFactory =
new SqlSessionFactoryBuilder().build(inputStream);
```

该XML配置文件包含了MyBatis的核心设置，包括生成数据库连接实例的数据源，以及确定事务范围和控制的事务管理器。该XML配置文件的全部细节都可以在本文件中找到，下列是一个简单的例子：

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <environments default="development">
        <environment id="development">
        <transactionManager type="JDBC"/>
            <dataSource type="POOLED">
            <property name="driver" value="${driver}"/>
            <property name="url" value="${url}"/>
            <property name="username" value="${username}"/>
            <property name="password" value="${password}"/>
        </dataSource>
        </environment>
    </environments>
    <mappers>
    	<mapper resource="org/mybatis/example/BlogMapper.xml"/>
    </mappers>
</configuration>
```

尽管还有更多关于XML配置文件的细节，但是上面这个例子指出了最至关重要的部分。XML文件头部用来验证XML文档。**environment**元素包含事务管理和连接池的环境配置。**mappers**元素包含了一系列的mapper--XML文件或Java注解中包含了SQL语句和映射的定义。

### Building SqlSessionFactory without XML

如果你更喜欢直接使用Java代码来构建配置，而不是使用XML，或者想创建自己的配置构建器，MyBatis提供了一个完成的配置类，该类提供了和XML文件中对应的所有配置选项。

```java
DataSource dataSource = BlogDataSourceFactory.getBlogDataSource();
TransactionFactory transactionFactory =
	new JdbcTransactionFactory();
Environment environment =
	new Environment("development", transactionFactory, dataSource);
Configuration configuration = new Configuration(environment);
configuration.addMapper(BlogMapper.class);
SqlSessionFactory sqlSessionFactory =
	new SqlSessionFactoryBuilder().build(configuration);
```

注意，在这个例子中，配置添加了一个mapper类。该mapper类包含了SQL映射注解，从而避免了使用XML。由于Java注解和一些MyBatis映射的复杂性的一些限制，为了实现最高级的映射（内置连接映射），仍然需要用到XML文件。为此，MyBatis会自动查找并加载与mapper类对等的XML文件，如果它存在的话（在上例中，会根据类路径和BlogMapper.class的名称加载BlogMapper.xml）。稍后详述。

### Acquiring a SqlSession from SqlSessionFactory

现在您已经有了一个SqlSessionFactory，顾名思义，你可以从中获取一个SqlSession实例。该实例包含了对数据库执行SQL命令的所有方法。你可以使用SqlSession直接执行SQL语句。例如：

```java
try (SqlSession session = sqlSessionFactory.openSession()) {
    Blog blog = session.selectOne(
    	"org.mybatis.example.BlogMapper.selectBlog", 101);
}
```

尽管这种方法能够起作用，并且对MyBatis之前版本的用户来说很熟悉，但是现在有一种更加优雅的方法：使用一个接口为一条给定的语句描述参数和返回值，这种方法相比之前的方式更优雅，也更类型安全，也不容易出错。例如：

```java
try (SqlSession session = sqlSessionFactory.openSession()) {
    BlogMapper mapper = session.getMapper(BlogMapper.class);
    Blog blog = mapper.selectBlog(101);
}
```

现在让我们探索上面的代码到底执行了什么。

### Exploring Mapped SQL Statements

此时，你可能很想知道SqlSession或者Mapper类到底执行了什么。映射SQL语句是一个很重要的主题，并且这一主题占据了本文档的大部分。但是为了让你了解到底执行了什么，这里有几个例子。

在上面的例子中，语句能够被定义在XML或注解中。让我们首先看在XML中，MyBatis提供的全套特性都能通过使用基于XML的映射语言实现，这也使得MyBatis流行了很多年。如果你之前使用过MyBatis，你应该对概念很熟悉，但是有很多关于XML映射文档的改进会在之后发布，这里有一个基于XML的映射语句的例子，符合上述SqlSession调用的例子。

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="org.mybatis.example.BlogMapper">
    <select id="selectBlog" resultType="Blog">
    	select * from Blog where id = #{id}
    </select>
</mapper>
```

尽管对于这个简单的例子来说花销很大，但实际上它很轻量级。只要你喜欢，你可以定义许多应映射语句在单个mapper XML文件中。文档的其余部分很容易解释。它定义了映射语句的名称为“selectBlog”，当前映射的命名空间为“org.mybatis.example.BlogMapper”，这可以使你通过指定“org.mybatis.example.BlogMapper.selectBlog”的全限定名来调用它，正如我们在下面列子中所做的那样:

```java
Blog blog = session.selectOne(
	"org.mybatis.example.BlogMapper.selectBlog", 101);
```

注意，这和在全限定Java类上调用方法是多么的相似，这是有原因的。映射语句的名称可以直接被映射到命名空间所对应的类的方法上，只要存在名称，参数和返回值类型匹配的方法。你可以很简单的调用Mapper接口中的方法，比如下面的例子：

```java
BlogMapper mapper = session.getMapper(BlogMapper.class);
Blog blog = mapper.selectBlog(101);
```

第二种方法有很多优势，首先，它不依赖于一串字符，所以更加安全，另外如果你的IDE有代码补全的功能，当你在浏览你的映射SQL语句时，你可以利用这样的功能。

==关于**namespace**的笔记==：

==命名空间==在之前的版本中时可选的，这很容易使人困惑且毫无帮助。命名空间现在要求为必需，为了实现通过类的全限定名来隔离语句的目的。

正如你所看见的，命名空间开启了接口绑定，即使你不认为你会在今天使用了它们。万一你改变注意，你应该遵循这里列出的这些做法。一旦使用了命名空间，并且将其放入合适的Java包命名空间中，会让你的代码更加整洁，从长远来看，也提高了MyBatis的可用性。

==名称解析==

为了减少打字量，MyBatis使用下列名称解析规则来解析所有的命名配置元素，包括语句，结果集和缓存等。

* 全限定名被直接查找且如果找到了会被使用。
* 

​	

### 作用域和生命周期

理解多种作用域和生命周期非常重要，如果不能正确使用它们，会导致严重的并发问题。

#### SqlSessionFactoryBuilder

一旦创建了SqlSessionFactory，就不再需要该类。因此该类实例化的最佳作用域是方法，即该类实例作为一个方法的本地变量。你可以使用SqlSessionFactoryBuilder构建多个SqlSessionFactory，但是最好还是不要保留它，以便确保所有的XML解析资源能够被释放掉，从而能够处理其他更为重要的事情。

#### SqlSessionFactory

该类的实例一旦创建了就应该在整个应用的执行期间存在。最佳实践表明在程序运行中，不要构建SqlSessionFactory多次。因此，SqlSessionFactory的作用域应该是整个应用。有很多种方式可以实现，最简单的方式就是使用单例模式或者静态单例模式。



#### SqlSession

每个线程应该有它自己的SqlSession实例。该实例不能被共享而且也是线程不安全的。因此，该实例的作用域最好为线程作用域。不要在静态作用域中使用该实例或将其作为类的属性字段。不要将该类型的实例放入任何类型的托管作用域中，比如HttpSession。如果你正在使用任何类型的web框架，请考虑将SqlSession的作用域调整为单个请求。

#### Mapper Instances

Mappers是一些接口，和映射语句相关联。mapper接口的实例从SqlSession中获取。同样地，mapper实例的最大作用域也和SqlSession的作用域一致，但是，mapper实例的最佳作用域是方法作用域。







# Configuration XML

## Configuration

### properties

### settings

### typeAlias



# Mapper XML Files



# Dynamic SQL



# Java API



# Statement Builders



# Logging

