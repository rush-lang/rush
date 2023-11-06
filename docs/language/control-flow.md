# Control Flow

Controlling flow in a program.

## Conditional Statement

### The If Statement

It's often useful to execute code conditionally. The _if_ statement is the most commonly used conditional processing statement. In it's simplest form, an if statement has a single condition and one more statements to execute.

```
if condition
	statements
```

```rush
let value = 5

if value < 10
	print("value is less than 10")
```

When the condition evaluates to `true` the statements in the body of the if will be executed; otherwise they will be skipped.

### The If-Else Statement

```
if condition
	statements
else
	alternative-statements
```

```rush
let value = 15

if value >= 0
	print("value is positive")
else
	print("value is negative")
```

```rush
if condition ## inline statement to execute if the condition is true.
```

## While Loops

```rush
while condition
	## ...
	## statements executed while the condition is true.
```

```rush
while condition ## inline statement to execute while condition is true
```

## While-Else

```
while <condition>
   <statements>
else
   <final/alternative-statements>
```

## Iterative Statements

## For-In Loops

```rush
for item in collection
	## ...
	## statements executed for each item in the range.
```

```rush
for item in collection ## inline statement to be executed for each item in the range.
```

## For-Else Statements

```rush
for i in 0..^3
	if 5 < i
		break
else
	print("less than 5")
```

## Control Transfer Statements

Control transfer statements alter the flow of program execution. There are five control transfer statements in Rush:

- `continue`
- `break`
- `return`
- `yield`
- `throw`
