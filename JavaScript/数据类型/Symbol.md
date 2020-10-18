# 简述 

假如目前有一个对象，我们想为其添加属性，但是不知道该属性是否已经存在了，此时就可以使用Symbol。

```js
let obj = {...};
const operations = {
	up: Symbol(),
   down: Symbol()
}
obj[operation.up] = function() {
    //...
}
obj[operation.down] = function() {
    //...
}
```

这样可以做到安全和高效，因为Symbol的独一无二的特性，我们不用担心属性被覆盖掉，同理，就不用再花时间来判断是否已经存在同样的属性了。



# Well-known Symbols

Symbol类型有一些已定义的属性，可以将其作为对象的属性以改变对象在某些特定场景下的行为。