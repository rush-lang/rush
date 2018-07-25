
If you do not define a constructor for a class, a default constructor will be generated.
The default constructor generated depends on the members of the class that must be
initalized (i.e. non-optionals) and the type of each member, this information is used
to form a parameter list.

```
class point:
    public var x, y: decimal

var p = new point(1, 2)
```

## Properties

class point:
    private var _x, _y: decimal

    public x: decimal
        get => _x
        set => _x = value

    public y: decimal
        get => _y
        set => _y = value

### Auto Properties

```
class point:
    public x: decimal => get set
    public y: decimal => get set

```

## Extensions Classes

Extension Classes allow you to extend the members of a class and provide a facility similar to Open Classes.
To create an extension class you begin with the class keyword, and the name of the class you want to extend prefixed with the '@' (extension) symbol.

```
// defines a class 'foo'
class foo:
    ...

// defines an extension of class 'foo'
class @foo:
    ...
```

An extension class may contain any member that an ordinary class could. However, with an extension class we have access to the extended class's 'this' and
publicly accessible members.

```
class foo:
    public bar: int => get set

class @foo
    public fizz: bool
        get => this.bar % 3
    public buzz: bool
        get => this.bar % 5

...

var f = new foo(15);
writeln(f.fizz) // >> true
writeln(f.buzz) // >> true
```

## Class Overloads (Specialization)?

Classes may be overloaded in much the same way functions and methods are overloaded,
the constructors of each class overload must differ in their parameter lists
to disambiguate between the overloads to instantiate.

```
// first overload
class point:
    public var x, y: decimal

// second overload
class point:
    public var x, y, z: decimal

var p1 = new point(0, 0)    // instantiates the first overload.
var p2 = new point(0, 0, 0) // instantiates the second overload.
```

Functions and methods that take overloaded classes as one or more of their parameters are a special case.
To determine which of the overloads the function takes, the body is analyzed for statements concerning
the instance of the overloads type, if a member of a specific overload is accessed that does not exist
in other overloads, the resolution set is reduced to only those overloads which contain the member. If
two or more class overloads can be



