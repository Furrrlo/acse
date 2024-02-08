The laboratory question must be answered taking into account the implementation of the Acse
compiler given with the exam text.

Modify the specification of the lexical analyser (_flex_ input) and the syntactic analyser (_bison_ input)
and any other source file required to extend the Lance language with the ability to perform multiple
assignments on the same line.

In the unmodified _Lance_ language, assignments can only specify one variable on the left side of the
"=" token, and one expression on the right-hand side of the "=" token. In addition to this syntax, comma-
separated lists of variables and comma-separated lists of expressions shall be allowed to appear on the
left and right sides of the "=" token respectively.

For an assignment to be syntactically correct, the length of the two lists must be the same. Additionally, the list on the left of the "=" token should not include array elements. When this is not the case,
the compilation shall fail with a syntax error.

The order of variables in the list matches the order of expressions. In other words, the first expression
is assigned to the first variable, the second expression is assigned to the second variable, and so on.
The order in which the assignments are made in the compiled code is unspecified. Hence, dependencies
between two expressions can be disregarded.

The following code snippet exemplifies the usage of the multiple assignment feature. In the snippet,
three variables (a, b and c) are assigned by the same statement to three different values determined by
the expressions on the right-hand side.
```c
int a, b, c, d;

d = 10;
a, b, c = 5, 44 / 6, d * 2;

write(a); // 5
write(b); // 7
write(c); // 20
```


**Hint**: Be careful to not introduce ambiguities in the grammar. Indeed, the case in which the list
of variables and expressions are of length one is already handled by the standard assignment operator
grammar. Therefore, if in your grammar the lists can be of length one, and no other modifications are
made to the pre-existing rules, the grammar will be ambiguous.

# ACSE (Advanced Compiler System for Education)

ACSE is a complete toolchain consisting of a compiler (named ACSE), an
assembler, and a simulator (named MACE). It provides a simple but reasonably
accurate sandbox for learning how compilers work.

ACSE was provided to students in "Formal Languages and Compilers (FLC)" course 
at Politecnico di Milano during the lab lessons, in order to be used
for the exam.

You can navigate through the different branches of this repository to see the 
different exercises I solved in order to prepare for the exam.

The `main` branch is mostly unchanged and is exactly as provided, with the only
exception of the additon this small section in the README and the `.devcontainer`
stuff which I use for my dev env.

## How to use ACSE

ACSE was tested on the following operating systems:

- **Linux** (any recent 32 bit or 64 bit distribution should work)
- **macOS** (any recent version should work)
- **Windows** (both 32 bit or 64 bit) when built with
  [MinGW](http://www.mingw.org) under [MSYS2](https://www.msys2.org), or inside
  **Windows Services for Linux** (WSL).

If you are using **Linux** or **macOS**, ACSE requires the following programs
to be installed:

- a **C compiler** (for example *GCC* or *clang*)
- **GNU bison**
- **GNU flex**

If you use **Windows**, first you must install either the
[MSYS2](https://www.msys2.org) environment or **Windows Services for Linux**
(WSL). Both MSYS2 and Windows Services for Linux (WSL) provide a Linux-like
environment inside of Windows.

Once you have installed either MSYS2 or WSL, you can use the following
instructions just as if you were using Linux or macOS.

### Building ACSE

To build the ACSE compiler toolchain, open a terminal and type:

      make

The built executables will be located in the `bin` directory.

### Testing ACSE

To compile some examples (located in the directory `tests`) type:

      make tests

### Using ACSE

In order to use the compiler/assembler/simulator, first you have
to export the directory `./bin` in your current `PATH` as follows:

      export PATH=`pwd`/bin:$PATH

You can compile and run new Lance programs in this way (suppose you
have saved a Lance program in `myprog.src`):

      acse myprog.src myprog.asm
      asm myprog.asm myprog.o
      mace myprog.o

You can invoke `acse`, `asm` and `mace` without setting `PATH` if you wish. In
that case you'll need to specify the path to where they are located.

For example, if the current directory is still the directory where you have
invoked `make` to build ACSE, you'll use the following commands:

      ./bin/acse myprog.src myprog.asm
      ./bin/asm myprog.asm myprog.o
      ./bin/mace myprog.o

Alternatively, you can add a test to the `tests` directory by following these
steps:

1. Create a new directory inside `tests`. You can choose whatever directory
   name you wish, as long as it is not `test`.
2. Move the source code files to be compiled inside the directory you have
   created. The extension of these files **must** be `.src`.
3. Run the command `make tests` to compile all tests, included the one you have
   just added.
   
The `make tests` command only runs the ACSE compiler and the assembler, you
will have to invoke the MACE simulator manually.
