# VVTBI (Very, Very, Tiny, Basic Interpreter)

A simple BASIC interpreter written in C++ that supports essential BASIC commands like PRINT, LET, IF/THEN, GOTO, and REM statements.

## Features

- Line number based statements
- PRINT command with string and variable support
- Variable assignments (LET statement)
- Conditional branching (IF/THEN)
- Comments (REM)
- Simple arithmetic expressions
- GOTO statements

## Building and Running

```bash
make clean
make
./vvtbi your_program.vvtb
```

## Example Program

```basic
REM This is a sample VVTB program
10 LET a = 5
20 PRINT "The value of a is: ", a
30 IF a > 3 THEN 50
40 PRINT "This won't be printed"
50 PRINT "Done!"
```

## License

This software is released under the Apache License, Version 2.0.
For details, see http://www.apache.org/licenses/LICENSE-2.0

## Acknowledgments

- Original VVTBI concept by Jahan Addison (Mentor) <jahan.addison[at]jacata[dot]me>
- Special thanks to the original author for the inspiration and basic interpreter design
- This version is maintained by Daniel <nnamdidaniel114[at]gmail.com>

## A Note from the Author

This implementation of VVTBI, while perhaps not the most efficient tokenization solution available today, serves as an educational exploration of interpreter design and C++ programming fundamentals. It demonstrates core concepts like:

- Lexical analysis
- Token-based parsing
- Basic control flow
- Error handling
- File I/O

Sometimes the longer path teaches us more than the shortcut. 😊

For questions or feedback: nnamdidaniel114[at]gmail.com
