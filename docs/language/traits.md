# Traits

Traits define a collection of properties, methods and operators, but unlike classes, structures and records, they cannot have backing fields or be instantiated. A type conforms to a trait when implementing each abstract declaration of the trait.

At a minimum, traits are defined with the keyword `trait` followed by an identifier.

```rush
trait any
```

## Implementing Traits

Traits may be implemented implicitly or explicitly.

The standard library defines the trait `formattable` with a single method `format`. It can be implemented to convert types to `string`.

```rush
trait formattable
   func format() -> string
```

Implementing this trait will allow passing instances of the type where a `formattable` is expected; such as to the `print` family of functions.

```rush
struct point(x, y: float)
   func format() => "($x, $y)"

func main
   let p = new point(2, 1)
   print(p)
```

### Explicit Trait Implementations

Traits may be implemented explicitly

```rush
struct point(x, y: float)
   implements formattable
      func format() => "($x, $y)"

func main
   let p = new point(3, 0)
   print(p)
```

## Subtyping

```rush
trait formattable
   func format() -> string
```

## Default Implementations

```rush
trait equatable
   operator this == (other: self) -> bool
   operator this != (other: self) => !(this == other)
```

## Generic Traits

```rush
trait iterator<T>
   get value -> T
   func next() -> bool
```

## Type Constraint Traits

```rush
struct trait value_type_trait
class trait reference_type_trait
```

```rush
### SOMETHING.. NEED TO THINK OF A NAME... ###
class service
   func perform() => noop

class dependent(svc: service trait)

## in test..

class mock
   func perform() => noop

let x = new dependent(new mock())
```

### Links

- Nominal and Structural Typing
