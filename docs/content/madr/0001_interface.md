---
title: 0001 Determine the Interface Properties of the Tool
authors:
  - joe_starr
status: accepted
---

## Context and Problem Statement

The CLI needs a collection of command line arguments to be useful. We need to decide on what the
shape of those arguments/options should be.

## Decision Drivers

- The interface should favor simplicity over features.
- The interface has to be fast and flexible.
- The interface has to be obvious.

## Decision Outcome

Multiple options satisfy our needs. We will choose the following:

1. Support both:
    1. `-n`: Number of words to generate, default 10
    1. input on standard in
1. output on standard out

Future support for `-i` and `-o` is straight forward but unneeded at this time. A TUI and GUI are
not useful.

## Considered Options

1. A GUI or TUI
1. `-i`: An input file  
1. `-o`: An output file  
1. input on standard in
1. output on standard out
1. `-n`: An ITT notation

## Pros and Cons of the Options

### GUI or TUI

- Good, easy for users to understand.
- Bad, lots of overhead.
- Bad, hard to chain with other tools.

### `-i` and `-o`

- Good, can batch process files with notations.  
- Good, inherently reproducible.
- Bad, hard to chain in a pipeline  
- Bad, redundant with `cat <> | itt2plp`

### `-n`

- Good, Easy to see what is being fed as input.
- Bad, hard to chain with other tools.

### stdin

- Good, simple
- Good, portable
- Bad, requires a special `EOF` terminal char

### stdout

- Good, simple
- Good, portable
- Good, no special memory management
- Bad, path handling can be challenging

## More Information

N/A
