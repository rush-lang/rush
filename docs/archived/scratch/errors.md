## Exceptions

## Try Expressions

```
let (val, err) = try parse("main() => 0")
if err != nil:
	...
```

```
let val = try? parse("main() => 0")
if val != nil:
	...
```

## Try-Catch Blocks

```
try:
	...
catch e: error:
	...
finally:
	...
```