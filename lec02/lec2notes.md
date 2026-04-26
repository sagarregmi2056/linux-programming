# Linux Compilation Pipeline Notes

## GCC is a Driver, Not Just a Compiler

`gcc` is mainly a **driver program**.

It calls multiple tools internally:

* Preprocessor (`cpp`)
* Compiler (`cc1`)
* Assembler (`as`)
* Linker (`ld`)

So the real flow is:

```text
hello.c
 → preprocessing
 → compilation
 → assembly
 → linking
 → executable
```

---

# 1. Preprocessing (`-E`)

## Command

```bash
gcc -E hello.c > hello.i
```

## What it does

Processes:

* `#include`
* `#define`
* `#ifdef`
* macro expansion

It does **not** compile.

It creates:

```text
hello.i
```

which is the preprocessed C source.

## Output redirection

```bash
1> hello.i
```

means redirect **stdout** to file.

Same as:

```bash
> hello.i
```

because `1` = stdout.

### File descriptors

* `0` → stdin
* `1` → stdout
* `2` → stderr

---

# 2. Compilation (`-S`)

## Command

```bash
gcc -S hello.i
```

## What it does

Converts preprocessed C code into:

```text
hello.s
```

which contains **assembly code**.

Still not executable.

---

# 3. Assembly (`-c`)

## Command

```bash
gcc -c hello.s
```

## What it does

Assembler converts assembly into:

```text
hello.o
```

This is an **object file**.

It contains:

* machine code
* symbol table
* relocation info
* unresolved references
* sections like `.text`, `.data`, `.bss`

It is **not runnable yet**.

---

# 4. Linking

## What linker does

The linker takes:

```text
hello.o
```

and creates the final executable.

## Main jobs of linker

### 1. Combine object files

Example:

```text
main.o + math.o + utils.o
```

into one executable.

---

### 2. Resolve symbols

Example:

```c
printf()
```

is not inside your code.

Linker finds it inside `libc`.

This is called **symbol resolution**.

---

### 3. Assign final addresses

Linker decides final memory addresses for:

* functions
* global variables
* library calls

This is called **relocation**.

---

### 4. Add startup code

Linux starts from:

```c
_start
```

—not directly from `main()`.

Startup files like:

* `crt1.o`
* `crti.o`
* `crtn.o`

prepare runtime and then call `main()`.

---

### 5. Produce executable

Final result:

```text
a.out
```

or custom name using:

```bash
gcc hello.c -o hello
```

---

# Dynamic vs Static Linking

---

# Dynamic Linking

## Command

```bash
gcc hello.c -o dynamicexe -lc
```

## What happens

Executable does **not** include full `libc`.

It stores references like:

```text
Need libc.so at runtime
```

At runtime Linux loads shared libraries.

### Result

Smaller executable.

Example:

```text
dynamicexe → 16 KB
```

---

# Static Linking

## Command

```bash
gcc hello.c -o staticexe -lc --static
```

## What happens

Library code gets copied into executable:

* `printf()`
* libc parts
* startup code
* runtime support

### Result

Larger executable.

Example:

```text
staticexe → 767 KB
```

---

# Checking Size

## Command

```bash
ls -lh dynamicexe staticexe
```

Shows human-readable sizes.

Example:

```text
16K dynamicexe
767K staticexe
```

---

# Inspect Linked Libraries

## Command

```bash
ldd dynamicexe
```

Expected output:

```text
libc.so.6
ld-linux-x86-64.so.2
```

This proves dynamic linking.

---

## Command

```bash
ldd staticexe
```

Expected output:

```text
not a dynamic executable
```

This proves static linking.

---

# Important Truth

Most Linux applications use **dynamic linking** because:

* smaller binaries
* shared updates
* automatic security patches
* less memory duplication

Static linking is common in:

* embedded systems
* rescue binaries
* some containers
* minimal deployment targets

---

# Final Pipeline Summary

```text
hello.c
   ↓
Preprocessor (-E)
   ↓
hello.i
   ↓
Compiler (-S)
   ↓
hello.s
   ↓
Assembler (-c)
   ↓
hello.o
   ↓
Linker
   ↓
Executable (a.out / hello)
```

