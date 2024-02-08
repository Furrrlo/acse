# TdE 13/06/2022

The laboratory question must be answered taking into account the implementation of the `Acse`
compiler given with the exam text.

Modify the specification of the lexical analyser (`flex` input) and the syntactic analyser (`bison`
input) and any other source file required to extend the `Lance` language with the __zip__ statement. The
`zip` statement takes two source arrays, and copies their elements to a destination array in pairs – that
is, by alternating elements from the first array and elements from the second.

The source arrays may be of different lengths, or shorter than required to fill the entire destination
array. In these cases, the number of pairs of values copied is determined by the length of the shortest
input array. The rest of the destination array is left unchanged. In alternative, the destination array
may be shorter than required to contain all the elements from the source arrays. In that case the zip
statement stops as soon as the last element of the destination array is written. If any of the identifiers
does not refer to an array, a syntax error is generated and compilation is stopped.

The syntax and operation of the `zip` statement is shown in the example below. In the first example,
the c array is filled with the contents of _a_ and _b_ alternatively, in the order in which they appear in the
syntax (first _a_, then _b_). The _a_ array is shorter than _b_, (5 elements vs. 6), so the length of _a_ determines
the number of pairs to be copied (5 pairs). The number of elements copied is 10 (5 from a, 5 from b)
and the rest of the destination array c is left unchanged. In the second example, a is filled with elements
from b and c alternatively. The number of pairs to be copied is determined by the shortest array (_b_) but
the _a_ array is shorter than required. As a result only 2 pairs are copied fully, and the last pair is copied
only half-way.
```c
int a[5], b[6], c[12];
/* a == [1, 2, 3, 4, 5] */
/* b == [10, 20, 30, 40, 50, 60] */
/* c == [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] */

c = zip(a, b);
/* c == [1, 10, 2, 20, 3, 30, 4, 40, 5, 50, 0, 0] */

a = zip(b, c);
/* a == [10, 1, 20, 10, 30] */
```

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
