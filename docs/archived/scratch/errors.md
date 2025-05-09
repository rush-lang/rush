## Exceptions

## Try Expressions

```rush
let result = try parse("main() => 0") ## returns union of result type and possible exceptions
if result is error != nil:
   print "error: $result.message"

print result
```

```rush
## not sure what I was doing here.
let val = try? parse("main() => 0")
if val != nil:
   print val
```

## Try-Catch Blocks

Perhaps rush should not support exception blocks in favour of a mixed Exception/Result pattern solution. For example when exceptions are thrown they bubble up the stack trace as normal but developers can use the `try` expression to get Result pattern like behaviour.
