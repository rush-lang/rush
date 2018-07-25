## Function Declarations

```
<function name> ( <parameters> ) <return type> :
	<statements>
```



## Object Extensions

```
func(src: @type):
	// Do stuff
```

## Currying

```
add(a, b: number) => a + b
```

Passing $1, $2, ... $n as a function call argument does not in fact perform a function call,
Instead the function is curried, and the result is another function with a reduced parameter set.

```
let add3 = add(3, $1)

let xs = [1...10].map(add3)
```


[IDEA]
When invoking a parameterless function you may omit the parentheses:
```
let l1 = a.length() 	// with parentheses
let l2 = b.length 	// without parentheses
```

When invoking a function that requires a zero single argument, you may provide a single expression without parentheses.
```
let ys = head(xs) // with parentheses
let zs = head xs	// with parentheses
```

Omitting the parentheses is also applicable to extension functions, uses the method call syntax
is used.

```
let ys = xs.map(x => pow(2, x)) 	// with parentheses
let zs = xs.map x => pow(2, x)	// without parentheses
```