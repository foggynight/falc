# falc

Stack-based postfix calculator.

All numbers are doubles, there's a variety of built-in mathematical functions,
and the calculator contains a dictionary allowing values to stored and fetched
using single letter variable names.


## Installation

Build the `falc` executable:

    make

Optional: Copy `falc` into a directory on the `PATH`:

    cp falc /usr/local/bin


## Usage

    falc

This calculator operates by storing and fetching arguments from a data stack.
When you enter a number, it is pushed onto the stack. When you enter the name of
an operation, its arguments are popped off the stack, the operation is executed,
and its return value is pushed onto the stack.

In addition to some mathematical functions, `falc` has various operations
for manipulating the data stack, such as:

- `drop`: Drop the top element of the data stack.
- `clear`: Drop all elements of the data stack.

There is also an operation which allows values to be bound to single letter
variable names: `store`, which stores the value in the dictionary under the key
of the single letter word which follows `store`.

    42 store x

The stored value may be retrieved by executing `x`, which looks up the value
stored in the dictionary under the `x` key and pushes it onto the data stack.


## License

Copyright (C) 2022 Robert Coffey

This is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License version 3.
