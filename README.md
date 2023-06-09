# Brainfuck Interpreters In Different Programming Language
This project contains Brainfuck interpreters written in different programming languages. Continue reading to learn more about this idea.

## What's Brainfuck?
Brainfuck is an esoteric programming language which has only eight instructions and has been proved to be Turing complete. That means any programming language that is Turing complete can be use to implement a Brainfuck interpreter, and a programming language that can be used to implement a Brainfuck interpreter is Turing complete. Brainfuck is simple. The eight instructions are shown as below with descriptions:
```
+: Add the value in the pointed slot by one.
-: Substract the value in the pointed slot by one.
<: Substract the pointer by one (i.e. move the pointer to the previous memory slot)
>: Add the pointer by one (i.e. move the pointer to the next memory slot)
[: If the value in the pointed slot is zero, jump past the matching bracket.
]: If the value in the pointed slot isn't zero, jump back the matching bracket.
.: Output the pointed slot.
,: Read input and set it to the pointed slot.
```

An example program could be found in `/c/examples/hello.bf`.

## Why Brainfuck?
In my personal opinion, writing a Brainfuck interpreter is a good practice to learn a programming language. It is not too complicated to understand and also requires many programming skills that will be used in developing other applications as well. Also, if someone who has learned some programming languages wants to learn a new programming language, reading the source code a Brainfuck interpreter in that language should help him quickly become familiar with that progrmaming language.

## Restrictions
We take the C language version as the standard (source code could be found in `/c`). Any interpreter in this repository must behave like the C version, that includes keeping the following behaviors the same:
- Command-line argument format;
- Help message;
- The output when given the same input;
The error message could be different.

When writing a interpreter in the programming language, you must write the interpreter entirely in that progrmaming language, not just translate the C version into that or port the C version to that programming language. You should entirely rewrite it, and use features in that progrmaming language. You should make the source code typical in that progrmaming language, not a C-style source code in other programming languages.

The name of the binary must be `<language name or commonly-used abbreviation>bfi`, where `<language name or commonly-used abbreviation>` should be replaced with the name of the language (in lower case) or the commonly used abbreviation for it (in the lowercase as well), like `jsbfi` for JavaScript version, `rsbfi` for Rust version, `cppbfi` for C++ version, etc.

## TODO
The following interpreter will be implemented by the author of this repository, any programming language that aren't in the list is open for pull requests.
Any content in this repository (including pull requests that are ready for merging) must satisfy the restrictions.

- [x] C
- [x] D
- [ ] Rust
- [ ] Go
- [ ] C++
- [ ] Java
- [ ] Swift


