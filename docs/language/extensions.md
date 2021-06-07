# Extensions

Extensions provide a mechanism to add functionality to an existing, class, structure, record, enumeration, or interface type. This includes types for which you don't have access to the original source code (known as retroactive modelling).

Rush provides two mechanisms to define extensions; type based, and function based extensions.

## Type Based Extensions

Type based extensions allow you to define any additional members to existing types except fields.

```rush
class @something_
public
	func method()
```

## Function Based Extensions

Function based extensions allow you to define a function that can be called using the member-access  syntax, or the regular function call syntax.

```rush
func foo(x: @something_)
	##
```

```rush
let s = new something_()
s.foo() 		## member-access syntax of a function.
```

