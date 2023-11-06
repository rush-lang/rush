# Functions

In general, a function declaration begins with the keyword `func` and consists of an [identifier](), optional formal [parameter list](), an optional explicit return type and either a [block or expression body]().

```
func <identifier> [ <parameter-list> ] [ "->" <type> ] [ <block-body> | <expression-body> ]
```

```rush
func add(a, b: int) => a + b
```
