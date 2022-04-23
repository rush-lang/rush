
## Integer Literals

```rush
let x = 123
let y = 1`000`000
```

## Floating Point Literals

## Character Literals

```rush
let a = 'a' ## single quotes are character literals only when they contain a single character code point
```

## String Literals

```rush
let x = "hello"	## default	 (single quotes needn't be escaped.)
let y = 'hello'	## alternate (double qoutes needn't be escaped.)
let z = `hello`	## verbatim  (no special escape characters; string as-is.)
```

## Regular Expression Literals

```rush
let re = /ab+c/i

if "abc".match(/ab+c/):
	println("matches!")

## equivalent to above
if "abc" ~= /ab+c/:
	println("matches!")
```

## Sequential Array Literals

```rush
let values = [1, 2, 3]
```

## Associative Array Literals

Can't use this syntax because I would want type syntax (e.g. `[string:int]`) to go with it, but we already have `[type:size]` for fixed size sequential arrays, making the syntax kind of awkward/confusing.

That said, it may be ok to have this syntax as `[type:type]` and `[type:integer]` are unambigous. (* `new [type:expression]` might be weird though)

```rush
let titles = [
	"mr": 1,
	"ms": 2,
	"miss": 3,
	"dr": 4
]

let dice_roll = [
	(1, 1): "snake eyes",
	(6, 6): "box cars"
]
```

## Range Literals

```rush
## inclusive range
let xs = 1...10

## exclusive range
## includes all integers up to but not including the value on the right
## useful for indexing sequential containers.
let len = xs.length
for i in 0..^len
	let x = xs[i]
```

```rush
## closed range
1...5 	## [ 1, 2, 3, 4, 5 ]

## [something] range
1..^5		## [ 1, 2, 3, 4 ]

## one-sided range
0..* 		## [ 0, 1, 2, ..., +inf ]
*..5		## [ -inf, ..., 3, 4, 5 ]
..^5		## [ -inf, ..., 2, 3, 4 ]
```

## Slice Literal

```rush
## Slice literals can only be created within subscript expressions.
##
let xs = [1, 2, 3, 4, 5, 6]

xs[1..3]		## [2, 3, 4]
xs[..2]		## [1, 2, 3]
xs[-3..]		## [4, 5, 6]

class container<T>
public
	get this[index: int] -> T
	get this[indices: srange] -> slice<T>
```

## Tuple Literals

```rush
## tuple
let foo = (1, "text")

## named tuple
let bar = (first: 1, second: "text")
```

## Object Literals

```rush
let address = {
	postcode = "ABC DEF",
	street = "Street",
	city: string = "City"
}
```

## Function Literals

