
## Variables

The keyword var, signals the beginning of a variable declaration list.  Following 'var' is a list of identifiers
which can be used to reference variables at later parts of the code.

Declaration and initialization of a single variable.

```
var x = 10
```

As stated previously, var signifies the beginning of a variable declaration _list_ meaning we can declare
and initialize more than one variable at a time.

```
var x, y = 0, 1
```

When the number of variable declarations is greater than the number of initializers, the last known is used
to initialize the remaining variables.

```
// here both x and y are initialized with the value '10'
var x, y = 10

// here 'x' is initialized with the value '0', 'y' and 'z' are initialized with the value '1'
var x, y, z = 0, 1
```



## Constants

Transistive Immutability?

```
let x = 10
let x: int = 10
```

## Member Variables

```
public var x = 1
public var x: int
public var x: int = 1
```

## Member Constants

