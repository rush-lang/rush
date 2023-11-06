# Tuples

A tuple is a compound data type that groups a fixed number of values (two or more) into a single value. They're useful when we need to pass multiple values around as a single unit &mdash; e.g. returning multiple values from function.

Tuples are created with the tuple literal syntax shown below &ndash;
```
let tuple = (value1, value2, ...valueN)
```

The following declares a constant `pair` that is assigned a tuple of type `(string, int)`

```rush
let pair = ("key", 123)
```

The individual values of tuple can be accessed via object [destructuring](). The values are destructured in the same order as they are defined in the tuple. This is unique to tuples.

```rush
let {key, value} = pair
```



```rush
let {_, value} = pair
```

```rush
let {file, error} = try open('filename.txt')
```

> **Note:** Tuples are useful for passing around temporary values, but generally they are not recommended to build complex data.
