# TdE 05/07/2022

The laboratory question must be answered taking into account the implementation of the `Acse`
compiler given with the exam text.

Modify the specification of the lexical analyser (`flex` input) and the syntactic analyser (`bison` input)
and any other source file required to extend the Lance language with the ability to count how many bits
set to 1 are present in the binary representation of an integer value.

This task is performed by a new operator, called __count_ones__, which takes an __arbitrary expression__
as its only argument. The value of the operator is the number of bits set to 1 in the binary representation of
the value of the expression. The implementation of the operator must perform constant folding whenever
possible and must handle negative numbers correctly considering their two’s complement representation.
Due to the fact that the `Lance` language only supports 32-bit sized integers, the maximum result of
`count_ones` must be 32.

The operation and syntax of the `count_ones` operator is shown in the example below. The variable
a is initialized with the value 123456, whose binary representation ($11110001001000000_2$) has 6 digits
set to 1. In the first assignment statement, the variable b is assigned with the value of an expression
containing `count_ones`. The value of `count_ones` is 6, as its argument is simply the value of a. The
rest of the expression adds 100 to this value so, in the end, b is assigned the value 106. In the second
assignment, the value of `count_ones` is 13, as the binary representation of the value of the expression
$a − 64 + 255$ is $11110001011111111_2$ which contains 13 'one' digits. Finally, in the third assignment, the
value −1 is represented in binary by 32 'one's, because of two’s complement notation. As a result, the
value of `count_ones` is 32.
```c
int a = 123456, b;

b = count_ones(a) + 100;
/* b == 106 */
b = count_ones(a - 64 + 255);
/* b == 13 */
b = count_ones(-1);
/* b == 32 */
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
