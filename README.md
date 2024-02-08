# TdE 20/01/2023

The laboratory question must be answered taking into account the implementation of the _Acse_
compiler given with the exam text.

Modify the specification of the lexical analyser (_flex_ input) and the syntactic analyser (_bison_
input) and any other source file required to extend the Lance language with the ability to check if
an array element exists or not before performing an assignment. This operation is performed by an
expression operator called __inbounds__. Its syntax consists of the `inbounds` keyword itself, followed by
two parameters enclosed in parenthesis: a _destination_ and a _source_, in this order. The parameters are
separated by "=", mimicking the syntax used for assignments. If the left-hand side of "=" is not a scalar
variable, or if the right-hand side of "=" is not an array element, then the compilation fails.

When inbounds appears in an expression, it verifies that the index of the specified array element
is within bounds, i.e. if the index is greater or equal than zero and lesser than the size of the array. If
this condition is satisfied, the operator copies the array element to the scalar and returns 1, otherwise it
returns zero and no copy is performed.

The following code snippet exemplifies the operation and syntax of inbounds. The first if statement
checks the validity of index 0 (zero) in array a. The index is within bounds, therefore the value of a[0],
which is 8, is put into v and the first section of the if is executed. In the following statement, inbounds
appears in the expression argument of a write and checks if the index −1 + v is valid. The variable v
has just been assigned to 8, so the index is 7. Since this index is too large, a zero is printed and v is
unmodified. Finally, the program uses inbounds to print the last 3 values in a. At every iteration the
index i is checked, and as soon as it is not in bounds the condition of the while statement becomes false
and the loop terminates.
```c
int v, i, a[5];
a[0]=8; a[1]=6; a[2]=9; a[3]=4; a[4]=7;

if (inbounds(v = a[0])) {
      write(1); // v = a[0], writes 1
} else {
      write(2);
}

write(inbounds(v = a[-1+v])); // check fails, writes 0

i = 2;
while (inbounds(v = a[i])) {
      write(v);
      i = i + 1;
} // the loop writes 9, 4, 7
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
