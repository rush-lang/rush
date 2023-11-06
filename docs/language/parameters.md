
# Position Parameters

```rush
func greet(name: string)
    print('hello, $name!')

greet('Bob')
```

# Named Parameters

```rush
## ...

greet(name: 'Bob')
```

```rush
func greet(@{name: string})
    print('hello, $name!')

greet(name: 'Bob')  ## OK
greet('Alice')      ## ERROR
```

## Optional Parameters

```rush
struct person(@{
    name: string,
    email? string,
    address = new address()
})

let p = new person(
    name: 'Bob'
    ## email is optional and does not need to be set, it defaults to nil and it's type becomes 'string?'.
    ## address is optional and does not need to be set, it is provided a default value in the parameter list.
)
```


## Destructure into named argument
```rush
let users = new list<user>
users.append(new (
   name: 'Alice',
   email: 'alice@rush-lang.org', {
      password_hash: hash,
      password_salt: salt,
   }: crypto.hash(password)
))
```
