# Extensions

Extensions provide a mechanism for adding functionality to an existing, class, structure, enumeration or trait. This includes types for which you don't have access to the original source code (known as retroactive modelling).

Rush provides two mechanisms to define extensions; type, and function extensions.

## Type Based Extensions

Type based extensions allow you to define any additional members of existing types, except fields.

```rush
extend foo
public
	func bar()
		## do something..
```

## Function Based Extensions

Function based extensions allow you to define a function that can be called using the member-access syntax, or the regular function call syntax.

```rush
func bar(x: extend foo)
	## do something..
```

```rush
let s = new foo()
s.bar() 		## member-access syntax of a function.
```


