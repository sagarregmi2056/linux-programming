# Linux System Programming Progress Notes

## Topics Covered So Far

This README summarizes what has been studied from the Linux System Programming course so far.

---

# 1. GCC Compilation Pipeline

Understanding that `gcc` is not just a compiler — it is a **driver program**.

It internally calls:

* Preprocessor (`cpp`)
* Compiler (`cc1`)
* Assembler (`as`)
* Linker (`ld`)

Pipeline:

```text
hello.c
 → preprocessing
 → compilation
 → assembly
 → linking
 → executable
```

Generated files:

```text
.c   → source code
.i   → preprocessed source
.s   → assembly code
.o   → object file
a.out / hello → executable
```

---

# 2. Preprocessing

Command:

```bash
gcc -E hello.c > hello.i
```

Learned:

* `#include`
* `#define`
* macro expansion
* conditional compilation

Also understood shell redirection:

```bash
>   == 1>
```

where:

* `0` → stdin
* `1` → stdout
* `2` → stderr

---

# 3. Compilation to Assembly

Command:

```bash
gcc -S hello.i
```

Output:

```text
hello.s
```

This is assembly generated from C source.

---

# 4. Assembly to Object File

Command:

```bash
gcc -c hello.s
```

Output:

```text
hello.o
```

Learned:

* `.o` = object file
* contains machine code
* contains symbols
* contains relocation info
* not executable yet

---

# 5. Linking

Learned what linker does:

## Symbol Resolution

Example:

```c
printf()
```

is resolved from `libc`.

---

## Relocation

Final memory addresses are assigned for:

* functions
* globals
* library calls

---

## Startup Code

Program starts from:

```c
_start
```

—not directly from `main()`.

Linker adds startup files like:

* crt1.o
* crti.o
* crtn.o

---

## Final Output

Creates executable:

```text
a.out
```

or custom output:

```bash
gcc hello.c -o hello
```

---

# 6. Static vs Dynamic Linking

## Dynamic Linking

```bash
gcc hello.c -o dynamicexe -lc
```

Result:

```text
small executable
```

Because shared libraries are loaded at runtime.

Observed size:

```text
16 KB
```

---

## Static Linking

```bash
gcc hello.c -o staticexe -lc --static
```

Result:

```text
large executable
```

Because library code is copied inside executable.

Observed size:

```text
767 KB
```

---

## Verification using `ldd`

Dynamic:

```bash
ldd dynamicexe
```

Static:

```bash
ldd staticexe
```

Expected:

```text
not a dynamic executable
```

---

# 7. Inspecting File Type

Command:

```bash
file hello
```

Observed:

```text
ELF 64-bit LSB pie executable
```

Learned:

* ELF = Executable and Linkable Format
* PIE = Position Independent Executable
* dynamically linked
* interpreter = dynamic loader
* not stripped = debug symbols still present

---

# 8. Debug Symbols with GDB

Command:

```bash
gcc -ggdb hello.c
```

Learned:

* adds debug information for GDB
* source line mapping
* variable names
* function names
* stack frame info

Sections added:

* `.debug_info`
* `.debug_line`
* `.debug_str`

---

# 9. ELF Inspection using `readelf`

## ELF Header

Command:

```bash
readelf -h hello.o
```

Observed:

```text
Type: REL (Relocatable file)
Entry point: 0x0
Program headers: 0
```

Learned:

* `.o` is relocatable
* not executable
* no entry point yet
* no program headers because OS does not load `.o`

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

Read-only data like string literals

### `.symtab`

Symbol table

### `.strtab`

String table

### `.rela.text`

Relocation information

This proves `.o` is unfinished before linking.

---

# 10. Program Headers vs Section Headers

## Section Headers

Used by:

```text
linker
```

Describe internal sections.

---

## Program Headers

Used by:

```text
kernel loader
```

Describe how executable should be loaded into memory.

---

Key understanding:

```text
.o → sections only
Executable → sections + program headers
```

---

# 11. GDB Disassembly

Command:

```bash
gdb -q ./a.out
```

Inside GDB:

```gdb
disassemble main
```

Learned how C becomes assembly.

Example instructions:

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

# Important Concepts Understood

## Function Prologue / Epilogue

```asm
push rbp
mov rbp, rsp
...
pop rbp
ret
```

---

## Calling Convention

First function arguments go into:

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

Used for dynamic linking.

Procedure Linkage Table.

Very important for runtime linking.

---

# Next Focus

Continue learning:

```text
File descriptors
open/read/write/close
fork/exec/wait
signals
pipes
threads
sockets
Makefiles
Linux Device Drivers
Yocto
Embedded Linux
```

---

# Core Principle

Real learning is not:

```text
watching videos
```

Real learning is:

```text
building programs
using gdb
using readelf
using objdump
using strace
breaking things and fixing them
```

