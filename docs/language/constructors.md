## Constructors

## Primary and Data Constructors

```rush
struct point(x, y: float)
```

```rush
struct point
	data construct (x, y: float)
      ## data constructors create properties on the
      ## enclosing type of the same name and type as
      ## the parameters with which they are initialized.
      ## Unlike primary constructors they may include
      ## a body, the contents of which is executed after
      ## the properties have been initialized (maybe.. needs work).
```

```rush
data class user(@{
   =id: int,
   role: string,
   email: string,
   created_at? datetime,
   updated_at? datetime,
})

data class user
   let =id: int
   let role: string

   ## no constructor provided: compiler will generate
   ## a primary constructor that initializes all data
   ## properties using named parameters of the same name.
```
