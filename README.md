# X8000 Virtual CPU + TASM Assembler

**Note: This is a personal project with no industrial or production-level use. It was created for learning and fun.**

---

## 📦 Overview

This repository contains a complete simulation of a custom virtual CPU architecture called **X8000**, along with an accompanying assembler named **TASM** (Tiny Assembler). It also includes a set of programs written in X8000 assembly and documentation of the architecture and instruction set.

Despite being considered a "failed project" from an industrial perspective, it demonstrates system-level thinking, low-level programming, and architecture design.

---

## 🧠 Why It Exists

The X8000 project was written from scratch in C as a way to:
- Simulate how a CPU works internally
- Understand memory/register/instruction handling
- Design an instruction set and custom syscall interface
- Explore what it takes to build a basic toolchain from zero

**It was also a highly enjoyable and motivating experience in system creation.**

---

## 🗂 Project Structure

```txt
.
├── x8000/          # The X8000 CPU engine (interpreter-based virtual CPU)
│   └── main.c
├── tasm/           # TASM: The assembler for the X8000 architecture
│   └── main.c
├── programs/       # Assembly source files written for X8000
├── doc.md          # Documentation on the instruction set and system design
├── Makefile        # Builds the assembler and engine
└── README.md       # You're here
````

---

## ⚙️ Features

* Custom-designed 8-bit instruction set
* Simulated registers, memory, and syscalls
* Full assembler that converts `.s` to machine code
* I/O syscalls for stdin, stdout, stderr
* Dynamic memory management (malloc, realloc, free)
* Execution loop with syscall-based program termination

---

## 🚫 Limitations

* Not intended for production or embedded use
* No optimizations or security features
* Does not emulate real hardware

This project is a **personal experiment**, and as such, it has **no industrial use**. It’s a "failed" project only in the sense that it was never intended to be deployed—but not in terms of educational value or technical creativity.

---

## ✅ How to Build

Make sure you have a GCC compiler installed.

```bash
make
```

This will:

* Build the `tasm` assembler
* Build the `x8000` CPU engine

---

## 📖 Documentation

See [doc.md](./doc.md) for:

* The X8000 architecture design
* Register descriptions
* Instruction set reference
* Syscall table and usage

---

## ❤️ Author's Note

> I wrote this project in a two days, driven by passion and curiosity. It was not only a technical project—it was a joyful experience. Even though it’s not an industry-grade tool, it reflects a deep love of system-building and a desire to create from the ground up.

---

## 📜 License

This project is released under the MIT License.

---
