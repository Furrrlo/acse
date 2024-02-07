# TdE 30/06/2023

The laboratory question must be answered taking into account the implementation of the Acse
compiler given with the exam text.

Modify the specification of the lexical analyser (flex input) and the syntactic analyser (bison input)
and any other source file required to extend the Lance language with support for a __software-emulated
division operator__.

The software-emulated division operator shall have the same functional behavior of regular division
for positive dividend and divisors. However, _no DIV instructions_ must be emitted by ACSE to translate
it to assembly language. The operation may have undefined behavior if either or both dividend and
divisor are negative — i.e. the implementation does not need to handle that case. Division by zero must
be handled by computing a quotient of $2^{31} − 1$. The proposed implementation should perform proper
constant folding. The code executed at compile time is not restricted in its use of division instructions.
Therefore the C division operator may be freely used in the implementation, while the following functions
are not allowed:
- `gen_div_instruction()`,
- `gen_divi_instruction()`,
- `handle_bin_numeric_op()` when the operator argument is `DIV`.

There are no constraints imposed regarding the run-time complexity of the software-emulated division.
The symbol associated to this new operator is a bracketed slash (__[/]__) instead of the familiar slash
(/). Its precedence and associativity are the same as non-emulated standard division, which is retained
and coexists with the new software-emulated one.

The following code snippet exemplifies the use of the new operator. As long as both `a` and `b` are
positive and $b \neq 0$, the snippet will always print `1` when executed.
```c
int a, b;

read(a);
read(b);
if (a / b == a [/] b) {
      write(1);
} else {
      write(0);
}
```

Tips: The simplest algorithm for computing division is called _repeated subtraction_. It involves sub-
tracting the divisor from the dividend until the divisor is smaller than the dividend. The quotient is
given by the number of subtractions performed.

In your solution you can write `INT_MAX` to refer to the constant $2^{31} − 1$.

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
