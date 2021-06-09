# Constants and Variables

Constants and variables are a common programming language construct that allow you to store values (such as the integer `10`, or the string `"hello"`) in memory. Once initialized, the value of a _constant_ cannot be changed, however, the value of a _variable_ can be re-assigned after initialization, as many times as you like.

## Declaring Constants and Variables

Declaring a constant or variable is done with the `let` and `var` keywords respectively. The following example demonstrates the declaration of a constant named `constant` and a variable named `variable`.

```rush
let constant = 10
var variable = "hello"
```
> Note <br> It is often considered good practice to prefer immutable over mutable state, as such prefering constants over variables is common practice in Rush, unless the mutability of a variable is strictly required.

## Type Annotation

You can annotate constant and variable declarations with type information to explicitly state the type of the declaration, as opposed to relying on type-inference to deduce the type from the initializer expression.

To annotate a constant or variable with type information, place a colon after the constant or variable name, followed by the name of the type to use. The following demonstrates the declaration of a variable annotated with the type `string`:

```rush
var message: string
```

The code above can be read as: "Declare a variable called `message` of type `string`."

The variable can now store any value of type `string`. You can think of it as the "type" or "kind" of a thing that can be stored.

The variable `message` can now be set to any value of type `string` as shown in the example below:

```rush
message = "hello"
```

> Note <br> In practice, it's rare to explicitly state the type of constants and variables. The type of a constant or variable can be inferred from the the initializer expression, which is usually required at the time of declaration due to _definitive initialization_.
## Definitive Initialization
