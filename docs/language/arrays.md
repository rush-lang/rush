# Arrays

```rush
let array = [1, 2, 3]

```

```rush
func foo(data: [[int]])
   ## subscripting can be chained with the comma operator
   ## for convenience, as opposed to writing 'data[0][1]'
   return data[0, 1]
```

## Indexing, Slicing and Ranges

```rush
let xs = [1, 2, 3, 4]

## Slices
let ys = xs[1:^2]  ## slice excluding the first and last elements.
let ys = xs[:^2]   ## slice from the beginning up until the 2nd from last element
let ys = xs[1:]    ## slice from the second element up to the remainder of the array.
```

## Array Initialization

```rush
let xs = [1, 2, 3]   ## array-literal
```

```rush
## new syntax cannot use array type syntatic sugar because
## there is ambiguity between the syntax of the
## inferred new type and initializer list.
let xs = new [int]   ## empty array
let ys = new [int:10]   ## array of 10 ints (constructed using default initializer for each element)
```

```rush
## instead we must use the named type for arrays.
let xs = new array<int> (size: 10)
```

```rush
## this is also true for tuples
let t1 = new (int, int)          ## tuple or inferred type?
let t2 = new tuple<int, int> ()  ## use this instead.
```

```rush
## fortunately, arrays and tuples have built in literal syntax
let xs = [1, 2, 3]
let t1 = (1, "two", 3.0)

let ys: [int:3] = [1, 2, 3]
let t2: (int, string, double) = (1, "two", 3.0)
```
