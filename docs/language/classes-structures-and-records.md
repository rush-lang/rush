# Structures and classes

Structures and classes are _the_ primary characteristic of object-oriented programming. They are a general-purpose mechanism for describing new types to a programming language's type system, and can be used to structure your program into a collection of orchestrated objects.

Structures and classes encapsulate fields, properties, and methods, collectively known as _members_. While structures and classes share much of their functionality, there are a few suttle but key differences.

- Classes define reference types, and structures define value types.
- Classes support inheritance, and by extension polymorphism.
- Structures with default constructors are default initializable.

## Reference types

A type that is defined with the keyword `class` is a _reference type_. At run-time, when you create an instance of a reference type, enough memory is allocated on the managed heap for that specific object. Any constant or variable assigned to the newly created instance holds only a reference to the location of said object.

## Declaring Classes

Classes are declared by using the `class` keyword followed by a unique identifier, as shown in the following example:

```rush
class person
	## class definition goes here.
	## fields, properties, methods, etc.
```

## Creating objects

```rush
let p = new person()
```

## Value types

A type that is defined with the keyword `struct` is a _value type_. At run-time, when yuo create an instance of a value type, enough memory is allocated on the stack for that specific object. Any constant or variable assigned to the newly created instance holds

## Declaring Structures

Structures are declared by using the `struct` keyword followed by a unique identifier, as shown in the following example:

```rush
class person
	## class definition goes here.
	## fields, properties, methods, etc.
```