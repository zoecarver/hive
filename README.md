# Hive

Hive is a low level, typesafe, toy language with similar syntax to python or lua. It uses flex and bison for lexing and parsing. It uses LLVM to generate IR code which can be compiled with clang. 

## Install

```bash
$ mkdir build && cd build
$ cmake ..
$ make main
$ ./main <path to input> <path to output>
```

## Project Layout

```
├── ast # all trees
    ├── BinOp # code for generating binary operators (`+`, `=`, etc.)
    │   ├── ast.cpp
    │   └── ast.h
    ├── ControlFlow # if, else, turnary, for (coming soon)
    │   ├── ast.cpp
    │   └── ast.h
    ├── Functions # functions, calls, externs, prototypes, closures (coming soon)
    │   ├── ast.cpp
    │   └── ast.h
    ├── Number # basic constant integers and doubles
    │   ├── ast.cpp
    │   └── ast.h
    ├── Variables # creating variables, pointers, arrays & calling variables
    │   ├── ast.cpp
    │   └── ast.h
    ├── ast.cpp # AST protocol & block AST
    └── ast.h
```

Above is the layout of the ASTs, other than that the layout should be pretty strate forward.

## Hive Language

Features and syntax of the hive language.

### Example

See `test/foo.hive`

```lua
extern print(int)
extern printd(double)

def add double a, double b -> double
    a + b
end

def simple double i
    printd(i)
end

def main int a
    if (a) print(2) else print(4) end

    x = if (1) 5.0 else 2.0 end
    x = add(x, 2.0)

    arr = [1.0, 2.0, 3.0, 4.0]
    printd(arr[1])

    ptr = x&
    printd(ptr*)
    printd(ptr[0])

    simple(x)
end
```

### Functions

* `def` + name + (`type` name, ...) + (`->` return type)
* body (last line is returned)
* end

### Arrays

Arrays are simply pointers. 

* array must be of the same time
* currently arrays are of a fixed size (this will change)
* see syntax above

### Types

#### Array

Because arrays are just pointers: `int*`

#### Numbers

`int`, `double`

`1`, `1.0`

#### Functions

`def`, `def (int) -> int`

### Pointers & Refrences

`value*` and `value&` respectivly. You can also use subscripting (`ptr[0]`).