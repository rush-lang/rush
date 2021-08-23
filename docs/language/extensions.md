# Extensions

Extensions provide a mechanism to add functionality to an existing, class, struct, record, interface, enumeration, or concept type. This includes types for which you don't have access to the original source code (known as retroactive modelling).

Rush provides two mechanisms to define extensions; type based, and function based extensions.

## Type Based Extensions

Type based extensions allow you to define any additional members of existing types, except fields.

```rush
class extends foo
public:
	func bar():
		## do something..
```

## Function Based Extensions

Function based extensions allow you to define a function that can be called using the member-access syntax, or the regular function call syntax.

```rush
func bar(x: extends foo):
	## do something..
```

```rush
let s = new foo()
s.bar() 		## member-access syntax of a function.
```


