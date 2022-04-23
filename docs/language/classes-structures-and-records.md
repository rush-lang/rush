# Structures and classes

Structures and classes are _the_ primary characteristic of object-oriented programming. They are a general-purpose mechanism for describing new types to a programming language's type system, and can be used to structure your program into a collection of orchestrated objects.

Structures and classes encapsulate fields, properties, and methods, collectively known as _members_. While structures and classes share much of their functionality, there are a few suttle but key differences.

- Classes define reference types, and structures define value types.
- Classes support inheritance, and by extension polymorphism.
- Structures with default constructors are default initializable.

## Declaring Structures and Classes

Classes are declared using the `class` keyword followed by a unique identifier:
Structures are declared by using the `struct` keyword followed by a unique identifier:

```rush
class my_class
	## class definition goes here.
	## fields, properties, methods, etc.

struct my_structure
	## struct definition goes here.
	## fields, properties, methods, etc.
```

## Creating Instances of Structures and Classes

```rush
let x = new my_class()
let y = new my_structure()
```

## Reference and Value Types

A type that is defined with the keyword `class` is a _reference type_. At run-time, when you create an instance of a reference type, enough memory is allocated on the managed heap for that specific object. Any constant or variable assigned to the newly created instance holds only a reference to the location of the newly constructed object. A count of the number of references to a class is maintained by the run-time, once each reference goes out of scope and the count reaches zero, the instance destructor is called, and the memory will be reclaimed by the garbage collector at some later stage of the programs execution.

A type that is defined with the keyword `struct` is a _value type_. At run-time, when you create an instance of a value type, enough memory is allocated on the stack for that specific object. Any constant or variable assigned to the newly created instance is responsible for the instance itself, once the constant or variable goes out of scope the instance destructor is called and the memory used by the instance is cleaned up.

