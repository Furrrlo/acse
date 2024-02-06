# TdE 25/01/2024

The laboratory question must be answered taking into account the implementation of the Acse compiler given with the exam text.

Modify the specification of the lexical analyser (flex input) and the syntactic analyser (bison input)
and any other source file required to extend the Lance language with the ability to evaluate iterated
expressions.

This operation is available through a new statement called repeat_exp, having the following syntax:
```c
repeat_exp(⟨var.⟩ = ⟨exp. 1 ⟩, ⟨exp. 2 ⟩, ⟨exp. 3 ⟩);
```

The repeat_exp statement implements a simple loop which, at every iteration, re-computes `⟨exp. 3 ⟩`
and assigns it to `⟨var.⟩`. `⟨var.⟩` is initialized to the value of `⟨exp. 1 ⟩` before the loop. The number of
iterations is controlled by the value of `⟨exp. 2 ⟩`.

If `⟨exp. 2 ⟩` is zero or negative, the loop should never iterate, leaving `⟨var.⟩` to its initial value, i.e.
`⟨exp. 1 ⟩`. Even though this is allowed by the syntax, assume that `⟨exp. 2 ⟩` does not use `⟨var.⟩` in its
definition. As a result you can assume that `⟨exp. 2 ⟩` doesn’t change during the execution of the loop.

The following code snippet exemplifies the operation of the `repeat_exp` statement. The first
`repeat_exp` statement initializes a to zero, then performs the assignment $a \leftarrow a + 1$ ten times —
as specified by the `⟨exp. 2 ⟩` argument — leaving $a = 10$ at the end. The second `repeat_exp` initializes
`b` to $a × 3 = 30$, but then `⟨exp. 2 ⟩` evaluates to −5, which is a negative value. As a result, `b` is left
equal to 30 and is not changed again. The third occurrence of `repeat_exp` implements a variant of the
well-known recurrent formula $x_{i+1} = \frac{1}{2}(x_i + \frac{N}{x_i})$ to compute $\sqrt{4096576}$.
```c
int a, b;

repeat_exp(a=0, 10, a+1);
// a == 10

repeat_exp(b=a*3, a-15, b-1);
// b == 30

repeat_exp(a=1000, 3, (a+4096576/a)/2);
// a == 2024
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
