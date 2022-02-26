# rpn-calc

Reverse Polish Notation (or postfix) calculator at the command line.

All numbers are doubles, there's a variety of built-in mathematical functions,
and the calculator contains a dictionary allowing values to stored and fetched
using single letter variable names.


## Installation

Build the `rpn-calc` executable:

    make

Optional: Copy `rpn-calc` into a directory on the `PATH`:

    cp rpn-calc /usr/local/bin

Note: Any name may be chosen for the executable, I prefer `rpn`:

    cp rpn-calc /usr/local/bin/rpn


## Usage

    rpn-calc


## License

Copyright (C) 2022 Robert Coffey

This is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License version 3.
