<br />
<picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/rush-lang/rush/master/docs/rush-logo-dark.svg">
   <source media="(prefers-color-scheme: light)" srcset="https://raw.githubusercontent.com/rush-lang/rush/master/docs/rush-logo-light.svg">
   <img alt="Rush Logo" height="64" src="https://raw.githubusercontent.com/rush-lang/rush/master/docs/rush-logo-light.svg">
</picture>
<span style="margin-left: 8px">
<img alt="GitHub Release" src="https://img.shields.io/github/v/release/rush-lang/rush">
</span>

A general purpose programming language.

---

| **Platform** | **Architecture** | **Compiler** | **Status** |
|:---|:---:|:---:|:---|
| **Ubuntu 20.04** | x86-64 | GNU GCC | [![linux-gcc](https://github.com/rush-lang/rush/actions/workflows/linux-gcc.yml/badge.svg)](https://github.com/rush-lang/rush/actions/workflows/linux-gcc.yml) |
| **Windows** | x86-64 | MSVC 2022 | [![win32-msvc](https://github.com/rush-lang/rush/actions/workflows/win32-msvc.yml/badge.svg)](https://github.com/rush-lang/rush/actions/workflows/win32-msvc.yml) |
| **MacOS** | x86-64 | Clang 14 | [![macos-clang](https://github.com/rush-lang/rush/actions/workflows/macos-clang.yml/badge.svg)](https://github.com/rush-lang/rush/actions/workflows/macos-clang.yml) |

Rush is a new programming language that builds atop many of today's most popular languages. It is intended to be a general purpose, strongly typed, multi-paradigm, and expressive language, rich with features that promote best practices and the convergence of Object Oriented and Functional programming techniques.

It is the hope of the Rush Programming Language non-profit organisation for Rush, in its simplicity, to be an effective teaching tool for budding programmers, while at the same time, provide a powerful set of features, and an enjoyable development experience for masters of the craft.

## Feature Highlights

#### Infer all the things!

Rush wants you to type as little as possible to get the job done, and to get it done well. As such type inference, among others, is featured heavily in Rush. If you've ever heard of the DRY principle (**D**on't **R**epeat **Y**ourself), this is DRY at the compiler level.

Take for example the following simple function definition that adds two integers, `a` and `b` together, and gives you back the result of that operation.

```rush
func add(a, b: int) => a + b
```

Every word or symbol in the above function definition has a real purpose for being. Alternatively, you could expand the definition into its full glory as shown below.

```rush
func add(a: int, b: int) -> int
   return a + b
```

But why repeat yourself when the compiler already knows what you intended?

#### Definitive Initialization

All variables, constants, and parameters, within Rush, collectively known as storage, must be initialized. Initialization must occur during declaration for locals or globals, invocation for function parameters, or in the case of member fields, the body of a type's constructor. This simple compiler enforced rule means that all named storage entities are guaranteed to have a value. Alleviating the need for what's known as null-checking, or worse, null reference exceptions; a common source of serious bugs in the software industry, eliminated.

Take for example, the definition of the `map` function, which takes as it's parameters a source collection `src`, and a transformative function `fn` to apply to each element of the collection.

```rush
func map<T, R>(src: @iterable<T>, fn: T -> R)
   for elem in src
      yield fn(elem)
```

This definition has been greatly simplified when compared to other programming languages, purely because it is guaranteed `src` and `fn` are valid values of the specified types. As such, we needn't check for that prior, and our code becomes shorter, easier to read, and understand.

If you need a named storage entity that may or may not contain a value, you can specify a variable, or parameter as optional using `?` next to the type annotation, like so:

```rush
var x: int? = nil    ## initialize an optional int 'x' with no value.
```

In this way we're being explicit that the variable or parameter may not contain a value and that it must also be explicitly checked for one before use.

#### More to come

## Building the Compiler and Tools

Currently, the easiest way to build the project is through the [docker images](https://hub.docker.com/repository/docker/rushlang/rush-ci-x86-64-linux-gnu) hosted on Docker Hub. If you want to build Rush outside of docker you can also follow the instructions laid out by the Dockerfiles found in this repository and the [rush-lang/docker](https://github.com/rush-lang/docker) repository hosted here on Github. Full documentation and instructions for building and contributing to Rush will be available soon.
