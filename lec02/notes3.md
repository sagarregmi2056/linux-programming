# GDB, Execution Flow, ELF and Runtime Notes

## 1. `.o` vs Executable

### `.o` = Object File

```text
.o = object file
```

It is a partially completed machine-code file.

It contains:

* machine code
* symbol table
* relocation info
* unresolved references
* sections like:

  * `.text`
  * `.data`
  * `.bss`
  * `.rodata`

Example:

```c
#include <stdio.h>

int main() {
    printf("Hello\n");
}
```

After:

```bash
gcc -c hello.c
```

we get:

```text
hello.o
```

It contains machine code for `main()` but `printf()` is still unresolved.

So:

```text
.o ≠ executable
```

---

## `a.out` = Default Executable

When we run:

```bash
gcc hello.c
```

GCC creates:

```text
a.out
```

This is the final linked executable.

If we use:

```bash
gcc hello.c -o hello
```

then output becomes:

```text
hello
```

not `hello.a.out`

---

# 2. Linking Makes Program Executable

Only after linking does the program become runnable.

Before linking:

```text
hello.o
```

After linking:

```text
hello / a.out
```

Linker does:

## Symbol Resolution

Example:

```c
printf()
```

resolved from `libc`

---

## Relocation

Assigns final addresses for:

* functions
* globals
* library calls

---

## Startup Code

Program starts from:

```c
_start
```

—not directly from `main()`

---

## Final ELF Creation

Now OS can load and execute it.

---

# 3. `file hello`

Command:

```bash
file hello
```

Example output:

```text
ELF 64-bit LSB pie executable
```

Learned:

* ELF = Executable and Linkable Format
* 64-bit = architecture
* LSB = little endian
* PIE = Position Independent Executable
* dynamically linked
* interpreter = dynamic loader
* not stripped = symbols/debug info still present

---

# 4. `readelf` Understanding

## ELF Header

Command:

```bash
readelf -h hello.o
```

Important output:

```text
Type: REL (Relocatable file)
Entry point address: 0x0
Program headers: 0
```

Meaning:

* `.o` is relocatable
* not executable
* no entry point yet
* no program headers because kernel does not load `.o`

---

## Section Headers

Command:

```bash
readelf -S hello.o
```

Important sections:

### `.text`

Machine code

### `.data`

Initialized globals

### `.bss`

Uninitialized globals

### `.rodata`

Read-only strings and constants

### `.symtab`

Symbol table

### `.strtab`

String table

### `.rela.text`

Relocation info

This proves linking is still pending.

---

# 5. Section Headers vs Program Headers

## Section Headers

Used by:

```text
linker
```

They describe internal sections.

---

## Program Headers

Used by:

```text
kernel loader
```

They describe:

* what to load into memory
* where to map it
* permissions (R/W/X)

---

## Why `.o` Has No Program Headers

Because:

```text
.o is not loaded by OS
```

It is only used by linker.

Executable gets program headers after linking.

---

# 6. Debug Symbols

Command:

```bash
gcc -ggdb hello.c
```

This adds debug info for GDB.

It helps debugger know:

* source lines
* variable names
* function names
* stack frame info

Extra sections:

* `.debug_info`
* `.debug_line`
* `.debug_str`

`-ggdb` improves debugging, not execution speed.

---

# 7. Using GDB

## Start GDB

```bash
gdb -q ./a.out
```

`-q` = quiet mode

---

## Breakpoint

```gdb
break main
```

This means:

```text
stop before entering main()
```

---

## Run Program

```gdb
run
```

Execution flow:

```text
ELF loads
→ dynamic loader loads libc
→ _start runs
→ runtime setup
→ main()
→ breakpoint hits
```

---

# 8. `info registers`

Command:

```gdb
info registers
```

Shows actual CPU state during execution.

Important registers:

---

## `rip`

```text
Instruction Pointer
```

Current instruction being executed.

Most important register.

---

## `rsp`

```text
Stack Pointer
```

Top of stack.

Used for:

* function calls
* return addresses
* local variables

---

## `rbp`

```text
Base Pointer
```

Used for stack frame reference.

Often created by:

```asm
push rbp
mov rbp, rsp
```

---

## `rdi`

```text
1st function argument
```

Example for:

```c
int main(int argc, char *argv[])
```

`argc` goes into `rdi`

---

## `rsi`

```text
2nd function argument
```

Usually `argv`

---

## `rax`

Used for:

* return values
* temporary work
* syscall returns

---

# 9. Disassembly

Command:

```gdb
disassemble main
```

Shows assembly version of `main()`.

Example:

```asm
push rbp
mov rbp, rsp
lea rax, [rip+offset]
mov rdi, rax
call puts@plt
mov eax, 0
ret
```

---

# Important Concepts

## Function Prologue

```asm
push rbp
mov rbp, rsp
```

Creates stack frame.

---

## Function Epilogue

```asm
pop rbp
ret
```

Cleans stack and returns.

---

## Calling Convention

First 6 arguments:

```text
rdi, rsi, rdx, rcx, r8, r9
```

Return value:

```text
eax
```

---

## PLT

```text
puts@plt
```

PLT = Procedure Linkage Table

Used for dynamic linking.

Very important later.

---

# 10. Breakpoint Confusion

When GDB shows:

```text
4 printf("welcome...")
```

it means:

```text
execution stopped BEFORE this line runs
```

It does NOT mean `printf()` already executed.

---

Then:

```gdb
c
```

means:

```text
continue execution
```

Now `printf()` actually runs.

That is why output appears after `c`.

---

# Final Important Understanding

## `readelf`

shows:

```text
file structure on disk
```

---

## `gdb`

shows:

```text
runtime state in memory
```

---

Simple view:

```text
readelf = blueprint
GDB = live execution
```

system call numbers
```text 
less /usr/include/x86_64-linux-gnu/asm/unistd_64.h
```


``` 
