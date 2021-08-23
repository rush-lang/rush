# Inheritance

Inheritance is a primary characteristic of object-oriented programming. It allows you to define a class with fields, properties, methods, operators, etc. - otherwise known as members - that other types can derive their implementations from.

Inheritance is defined by a _base class_ and a _derived class_. Derived classes can call and access the members of their base class, and override `virtual` or `abstract` members to refine or modify their behaviour.

A class may only derive a single base class directly. However, inheritance is transistive. If class `C` derives class `B`, and class `B` derives class `A`, class `C` inherits the members declared in class' `B` and `A`.

> **Note** <br> Struct and record types do not support inheritance but they may implement interfaces.

## Defining a Base Class

As all classes are closed by default, the base class must be defined with the keyword `base` followed by `class`, in order to derive other types from it.

The following example defines a `base class` called `animal`. Further more, the animal base class defines a `virtual` method called `speak` that does nothing for instances of the animal class.

```rush
base class animal
public:
	virtual func speak():
		## do nothing
```

As with regular classes, you can create instances of a base class using the initializer syntax; the keyword `new` followed by a class name and parentheses.

```rush
let a = new animal()
```

## Deriving a Base Class

The following example defines a class, `dog`, that derives the base class `animal`. The dog class also defines an overridden implementation of the base class' `speak` method, with behaviour specific to the dogs.

```rush
class dog inherits animal
public:
	override func speak():
		println("woof!")
```

We can now create an instance of the dog class and call the `speak` method to print the string `"woof!"` to the console.

```rush
let d = new dog()	## instatiates a new instance of dog.
d.speak() 			## prints "woof!" to the console.
```

## Polymorphism

The above example partially demonstrates a key concept of inheritance; _Polymorphism_. Polymorphic classes substitute the implementation of one or more methods, properties, and operators. When instances of derived classes are referenced via the base class type, the overridden member implementation in the derived class will be invoked.

```rush
class cat inherits animal
public:
	override func speak():
		println("meow!")

var a = new dog() as animal ## 'a' is a reference to an 'animal' that points to an instance of 'dog'.
a.speak() ## prints 'woof!' to the console.

a = new cat() ## 'a' is now a reference to an 'animal' that points to an instance of 'cat'
a.speak() ## prints 'meow!' to the console.
```

## Abstract Base Classes