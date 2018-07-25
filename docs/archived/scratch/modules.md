## Modules

### The Import Statement

```
import std.io

...

writeln("hello world!")
```

### Scope Resolution

```
io::writeln("hello world!")
```

```
std::io::writeln("hello world!")
```

### Module Aliases

```
import std.io in x
import other.io

...

let a = x::readln()
```

### Custom Modules

```
module my.module

export add(a, b: numerical) => a + b
```