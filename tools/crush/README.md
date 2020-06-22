# Rush Compiler

This directory contains the source code for the Rush Programming Language Compiler.

## Usage

The crush compiler accepts a number of options and one or more files to be compiled into either a library, or runtime executable, depending on the options passed.

```
crush [options] file...
```

For example, to compile two files, `a.rush` and `b.rush`, into an executable format, the following will suffice.

```
crush a.rush b.rush
```

The above will produce an executable file named `out` by default. You can change the name of the output file with the `--out` option, like so.

```
crush --out my-program ...
```

For a list of all other options available with the CRush compiler, use the `--help` option on the command line, as below, or checkout out the [documentation](https://rush-lang.org/docs/compiler/options).

```
crush --help
```
