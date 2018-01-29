# psplit
Very basic command line tool to split texts based on patterns.

## Usage
psplit -p "pattern" "myfile.txt"

Currently supported patterns:
sentence    Split text by sentences

All options:
-p          Pattern, default is "sentence"
-t          Where to store the files to
-w          Skip CR and LF and double whitespaces
-h          Display this help.

## Build
No special libraries are needed. On a Linux system, compile it with just
`gcc psplit.c -o psplit.c`

## Attention!
This piece of code is not well tested and, as said above, very very basic.
