## Explicitly Scoped Variables
## deterministic disposal of resources

The lifetime of variables is generally non-deterministic and left to the runtime and garbage collector to determine when to reclaim memory; particularly for [reference types](./reference-types.doc).

However, Rush provides a mechanism for....

The `with` statement declares one or more variables that are scoped to it's own block body, and destructed once the end of the `with` statement block has been reached.

General form
```
with <storage-pattern>
	<statement>*
```

```rush
with x = new foo()
	## statements
```

It is rarely necessary to control the life-time of an object to such a degree. But in situations where some operation must be performed before an object goes out of scope, it can be quite useful. For example, a `file_stream` will usually need to be flushed and closed before going out of scope. You could call these methods on the instance manually like so.

```rush
## get file_stream
let fs = open("filename.txt")
try
   fs.writeln("some text")
finally
   fs.flush()
   fs.close()
```

But since the `file_stream` type calls these methods in it's destructor, you could use a with statement for cleaner less error prone approach.

```rush
with fs = open("filename.txt")
   fs.writeln("some text")
```
