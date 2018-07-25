```
uadd(a, b: numerical && !signable) => a + b
```

## Discriminated Unions(?)
http://www.drdobbs.com/cpp/discriminated-unions-i/184403821?pgno=2

```
concept floating:
	typeof: float | double

concept unsigned:
	typeof: ubyte | ushort | uint | ulong

concept integral: unsigned
	typeof: byte | short | int | long
```

```
concept any:

concept equatable:
	requires:
		a == b => bool

concept orderable:
	requires:
		a < b => bool

concept signable:
	requires:
		-a => signable
		+a => signable

concept incrementable:
	requires:
		++a => incrementable
		a++ => incrementable

concept decrementable:
	requires:
		--a => decrementable
		a-- => decrementable

concept numerical:	equatable, orderable, signable, incrementable, decrementable
	requires:
		a + b => numerical
		a - b => numerical
		a * b => numerical
		a / b => numerical
```