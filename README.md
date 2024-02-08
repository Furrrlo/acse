# TdE 08/06/2023

The laboratory question must be answered taking into account the implementation of the `Acse`
compiler given with the exam text.

Modify the specification of the lexical analyser (`flex` input) and the syntactic analyser (`bison` input)
and any other source file required to extend the *Lance* language with the ability to search an array for
elements with a given value, and then reassign them with another value.

This operation is performed using a new statement called **replace**. The syntax of the statement is
the following:
```c
replace(⟨array⟩, ⟨exp. 1 ⟩, ⟨exp. 2 ⟩);
```

The first argument, `⟨array⟩`, is the identifier of the array being modified by the statement. The second
argument, `⟨exp. 1 ⟩`, is the expression whose value must be searched in the array. The third and last
argument, `⟨exp. 2 ⟩`, is the new value that will be assigned to all items found with the same value as
`⟨exp. 1 ⟩`. If no elements of the array have the same value of `⟨exp. 1 ⟩`, the array is left unchanged.
Additionally, if `⟨array⟩` is not a valid array identifier, a syntax error is raised at compile-time.

The following code snippet exemplifies the use of the statement. Initially, we assume the contents
of array a are initialized to the integer numbers from 1 to 10 (the code to initialize a is not shown for
brevity). The first use of replace replaces all items in a whose value is $b − 17 = 7$, with 5. Only the
item at index 6 is equal to 7, therefore its value is replaced with 5. The second use of replace replaces
all items whose value is 5 with $\frac{b}{2} = 12$. The items at index 4 and 6 satisfy the condition and are indeed
replaced with the value 12. Finally, the third `replace` finds all items of value $−b × 10 = −120$ and
replaces them with $−24 + b = 0$. Since there are no items in the array with the specified value, the array
stays unchanged.
```c
int a[10], b=24;

/* a == {1, 2, 3, 4, 5, 6, 7, 8, 9,10}; */
replace(a, b-17, 5);
/* a == {1, 2, 3, 4, 5, 6, 5, 8, 9,10}; */
replace(a, 5, b/2);
/* a == {1, 2, 3, 4,12, 6,12, 8, 9,10}; */
replace(a, (-b)*10, -24+b);
/* a == {1, 2, 3, 4,12, 6,12, 8, 9,10}; */
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
