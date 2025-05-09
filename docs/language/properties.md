# Properties

A property is a type of [member](./members.md) that provides a syntactically convenient way of accessing stored and computed values. Stored properties store constant and variable values called fields, whereas computed properties return a computed value without storing anything.

In general, properties are bound to an instance of a type, however `static` properties are bound to the type itself and can be accessed without an instance.

\[property observers?\]

## Properties Overview

 - Properties are a convenient way of publicly exposing getters and setters values
 - Property getters defined by the `get` keyword return the property's value, and setters defined by the `set` keyword assign values to the property.
 - The context sensitive keyword `field` represents the underlying field of stored properties in the `get` and `set` accessors.
 - The context sensitive keyword `value`
 - The `get` and `set` property accessors have the context sensitive keywords `field` and `value`. `field` represents the underlying value of the property, and `value` (exclusive to the `set` accessor) represents the value assigned to the property.


## Stored Properties

A stored property is a constant or variable declaration of storage, where the keyword `var` defines a variable stored property, and the keyword `let` defines a constant stored property.

The following example defines a class called `person` that defines a variable stored property `x` and a constant stored property `y`.

```rush
class person
public
	var name: string
```


## Stored Property Initializers

Stored properties maybe initialized in a number of ways; at time of declaration, in the types constructor, or during instance creation with the property initializer syntax.

Constant properties may only be initialized once... subsequent attempts to initialize a constant property will fail.

```rush
class foo
public
   var property: string = "hello"

   construct (value: string)
      this.property = value
```
## Accessing Properties

As with all public members, accessing properties begins with the instance variable, followed by the '`.`' operator, followed by the name of the property. Properties that define `get` access, (automatic with `let`, `var`, or explicit `get` accessor)

```rush
let p = new person
p.name
```

Properties that define `set` access may be assigned to with the `=` operator.

```rush
let p = new person
p.name = "Denis Ritchie"
```

### Static Properties

```rush
class foo
public
   static let property: int = 0

## ...

let x = foo.property
```

## Computed Properties

```rush
struct vector2
   ## ...
   let length: float
      get => sqrt(x^2 + y^2)
      set => this /= length *= value

```
### Alternate Syntax (Shorthand)

Generally more useful when specifying only one of either accessor; `get`, or `set`.

```rush
struct vector2
   ## ...
   get length => sqrt(x^2 + y^2)
   set length => this /= length *= value

## the following demonstrates how to specify the type of property getter/setter
## explicit.

class foo
private
   var _bar: int

public
   get bar -> int => _bar
   set bar(int) => _bar = value
```


```rush
struct small_number
public
   private var number: int
   private(set) var projectedValue: float

   ## or..

   var projectedValue: float
      get => field
      private set => field = value
```

## Constant Properties

Constant properties, usually declared with the `let` keyword, may also be declared using the `const` keyword. Constant properties declared with the `const` keyword may not be initialized via the constructor, instead they must be initialized at the point of declaration. After initialization, `const` and `let` properties are semantically equivalent.

This syntax exists for clarity between storage intended to be initialized by a user of the type, or a static value of the type itself.

> - Properties declared with the `const` keyword may not define a property getter or setter.
> - Properties declared with the `const` keyword must be initialized with a compile time constant value.


```rush
class foo
public
   const bar = 'constant value'
   construct ()
      bar = 'some new value'  ## ERROR: bar declared 'const'
```

```rush
class foo
   ## full property definition.
   prop x: int
      get => field
      set => field = value

   ## short hand for get only prop
   ## C#: public int x { get; }
   get x -> int
      return field

   ## short hand for set only prop
   ## C#: public int x { set; }
   set x(int)
      field = value

   ## variable auto-property definition
   ## C#: public int x { get; set; }
   var x: int

   ## constant auto-property definition
   ## C#: public int x { get; init; }
   let x: int

   ## immutable auto-property definition
   const x: reference_type
```
