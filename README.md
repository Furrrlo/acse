# TdE 04/09/2023

The laboratory question must be answered taking into account the implementation 
of the Acse compiler given with the exam text.

Modify the specification of the lexical analyser (`flex` input) and the 
syntactic analyser (`bison` input) and any other source file required to 
extend the Lance language with support for the `compare-and-swap` primitive.

This primitive will be available through a new statement called cswap having 
the following syntax:
```c
cswap(⟨var. 1 ⟩, ⟨exp. 1 ⟩, ⟨exp. 2 ⟩, ⟨var. 2 ⟩);
```

The statement, when executed, performs the following operations. First, it 
checks whether variable `⟨var. 1 ⟩` is equal to expression `⟨exp. 1 ⟩`. If the
two are indeed equal, the statement assigns the value of expression `⟨exp. 2 ⟩` 
to `⟨var. 1 ⟩`, and assigns the old value of `⟨var. 1 ⟩` to variable `⟨var. 2 ⟩`. 
On the contrary, if the value of `⟨var. 1 ⟩` is different from the value 
of `⟨exp. 1 ⟩`, `⟨var. 1 ⟩` is left unmodified, and its value is also assigned 
to `⟨var. 2 ⟩`. If both `⟨var. 1 ⟩` and `⟨var. 2 ⟩` refer to the same variable, 
the behavior of the statement is undefined (in other words the implementation 
doesn’t need to handle this case).

The following short program exemplifies the operation of the `cswap` statement. 
Initially the program declares two variables `a` and `b`, with a immediately 
initialized to 10 and b left uninitialized. Then, the `cswap` statement is 
used to compare a with 10. Since a is indeed equal to 10, a is assigned the 
value 11, and b is assigned the old value of a, therefore b = 10. At this point 
`cswap` is used again to compare a with 12. However, a is equal to 11 and not 
12, therefore a is left unmodified and b is assigned the value 11.

```c
int a, b;
a = 10;

cswap(a, 10, 11, b); // compare successful
// a == 11, b == 10

cswap(a, 12, 15*2, b); // compare failed
// a == 11, b == 11
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
