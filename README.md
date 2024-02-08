# TdE 05/09/2022

The laboratory question must be answered taking into account the implementation of the `Acse`
compiler given with the exam text.

Modify the specification of the lexical analyser (`flex` input) and the syntactic analyser (`bison`
input) and any other source file required to extend the Lance language with the ability of generating
pseudo-random numbers.

The implementation must define the __randomize__ statement and the __random__ operator. When `random`
appears in an expression, it generates a new random number from the last one returned (denoted _n_ in
the following) using the following formula:
$$(1664525 × n + 1013904223) \mod 2^{32}$$

The formula above is called a _Linear Congruential Generator_ and the initial value of _n_ is set to 12345.
Since all integers in ACSE are 32-bit in size, there is no need to explicitly compute the modulo operator.
The `randomize` statement resets n to the value of the expression enclosed between the parenthesis of
the statement.

The following code snippet exemplifies the operation of `random` and `randomize`. In the loop at the
beginning of the example, the first three random numbers from the sequence are extracted and printed.
These numbers are 87628868, 71072467 and −1962130922, respectively. Indeed, 
$(1664525 × 12345 + 1013904223) \mod 2^{32} = 87628868$. 
The other two numbers are obtained in the same way, but replacing
12345 with 87628868 and 71072467 respectively. After the loop, the `randomize` statement is used to
reset _n_ to the value of the expression 12300 + 45, which is equal to 12345. Since _n_ now is equal to its
initial value, the subsequent use of `random` returns the first number in the sequence again, 87628868.
Finally, `random` is used inside an expression. The current value of `random` is 71072467; this value is
then divided by 1000 and added to 7, resulting in the value 71079 which is then assigned to the variable
_a_. The value of _a_ is then printed to the output.
```c
int a, i;

i = 0;
while (i < 3) {
      write(random());        // 87628868, 71072467, -1962130922
      i = i + 1;
}

randomize(12300 + 45);
write(random());              // writes 87628868
a = random() / 1000 + 7;
write(a);                     // writes 71072467 / 1000 + 7 = 71079
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
