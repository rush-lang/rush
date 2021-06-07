# Concepts

A Concept is a boolean predicate of the type system, that states zero or more expressions must be valid for a particular type, or set of types. They are generally used as constraints of generic type parameters and can be applied in a number scenarios. Generic type parameters can be thought of as a way to punch a hole in a statically typed system, while concepts can be thought of as a way to define the shape and size of that hole.

## Defining a Concept

To define a concept, begin with the keyword `concept`, the name, and it's parameters. The body of the concept is defined by a single requires block.

```rush
concept <T> hashable(a: T)
requires:
	hash(a) -> uint
```

The above code snippet defines a generic concept named `hashable` with a requires block that states: Instances of type `T` must conform to the expression `hash(a)` that results in an instance of type `uint` to satisfy the constraint `hashable`.

## Extending Concepts

Concepts may be extended in one of two ways; a short form where parameter types of the concept are they themselves other concepts, or the long form using the requires block. Both forms are shown below.

```rush
# Short-form concept extension

concept integral(a, b: numerical)
requires:
	...
```

```rush
# Long-form concept extension.

concept <T> comparable(a, b: T)
requires:
	equatable(a, b) => true
	orderable(a, b) => true
	...
```

## Recursive Concept Constraints

Concepts may define constraints that require an expression to result in a type that satisfies the concept being defined.
During compilation a list of types that have satisfied or have not satisfied the constraints is maintained and used to break from the recursion. If the result type of the expression is known to satisfy constraints or not, or the result type is the same as the type of parameters being tested, the recursion stops.

```rush
concept integral(a, b: numerical)
requires:
	a % b -> integral
	...
```

## Applying Generic Type constraints with Concepts

Once a concept has been defined, it can then be used to apply constraints to the generic type parameters of classes, structs, interfaces, functions and methods.

### Functions and Methods

Similar to concepts themselves, functions and methods may define constraints of generic type parameters in one of two ways.
Parameters may be annotated with concepts, or a `requires` clause may be supplied after the function parameter list and before the return type annotation (if any).

```rush
func sort(xs: [orderable]):
   ...
```

```rush
func <T> sort(xs: [T]) requires orderable<T> -> [T]:
   ...
```

In the case of methods defined in a generic class, struct, or interface:

```rush
class <T> foo:
public:
   func bar() requires constraint<T>:
      ...
```

`bar` defines a method callable only when `T` is a of a type that satisfies the `constraint`.


### Classes, Structs, and Interfaces

To apply constraints over the generic type parameters of a class, struct or interface, add a `requires` clause before defining the class body with `public`, `private`, `protected`, `internal` member sections, or an indentation.

```rush
class <T> sorter
requires orderable<T>
public:
   func sort(xs: [T]):
      ...
```

### Conjunctive Concepts

Type constraints may be applied as the conjunction of two or more concepts.

```rush
constraint1 & constraint2 ... & constraintN
```

A type must satisfy each constraint to satisfy the conjunction as a whole.
