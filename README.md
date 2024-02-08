# TdE 07/02/2022

The laboratory question must be answered taking into account the implementation of the `Acse`
compiler given with the exam text.

Modify the specification of the lexical analyser (`flex` input) and the syntactic analyser (`bison` input)
and any other source file required to extend the `Lance` language with the ability to execute code blocks
inside expressions.

Code blocks appear inside a special operator called __exec__, which can be _nested_: in other words, an
`exec` operator can appear inside the code block of another `exec` operator. At runtime, when the `exec`
operator is evaluated, the code block that it contains is executed.

The _return_ statement is used to set the value of the innermost enclosing `exec` operator expression.
To allow this, a new alternative syntax for `return` statements introduces the ability to specify an
expression immediately after the `return` keyword. This kind of `return` statement is illegal outside an
`exec` operator, and must generate a syntax error. After setting the value of the operator, this new kind
of `return` statement interrupts the execution of the block; in other words, it jumps to its end. If no
`return` statement appears in the block, the value of the `exec` operator will be zero.

The syntax and semantics of the `exec` operator is exemplified in the following snippet. When the
`exec` operator in the expression is evaluated, its code block is executed. Since _b_ is equal to 5 the body
of the second `if` statement is run, causing the code block to return the value 5. At this point, the block
exits, and the last statement in it is not executed. The rest of the expression is evaluated, and the value
returned by the block is multiplied by 2. This value is assigned to the _a_ variable and then printed to the
output.
```c
int a, b;

b = 5;
a = exec({
      if (b > 10)
        return b * 2 - 10;
      if (b > 0)
        return b;
      return 0;
    }) * 2;

write(a); // 10
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
