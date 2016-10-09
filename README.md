ROSE
====

**ROSE** is a programming platform designed as a convinent low-level layer with some high-level features. Programs are represented as [modules](doc/module.md) with ROSE [bytecode](doc/instr.md).

ROSE **virtual machine** is a program runtime - basically, it is a bytecode interpreter. It loads, verifies and executes modules.

ROSE [assembler](doc/rasm.md) can be used for textual module representation, and **rasm** is an assembler translator.

Some ROSE features:
* it tries to be fast, small and extremely portable;
* code verification: no code can damage VM;
* modularity and late binding;
* garbage collector;
* unified interface for different tasks through extended filesystem.

Many ideas are derived from UNIX, Plan9 and JVM.

[Documentation](doc/index.md)

**License**: MIT.
