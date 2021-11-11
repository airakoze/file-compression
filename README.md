# File Compression with system calls

rle - compress or decompress a file with run-length encoding

# Usage

rle "input file" "output file" "compression length" "mode"

input file: the file to compress/decompress

output file: the result of the operation

compression length: the base size of candidate runs

mode: specifies whether to compress or decompress- if mode=0, then compress the input file, if mode=1 then decompress the input file

# Description

rle implements run-length encoding for the compression of files. You must use the read() and write() system calls (documented at man 2 read and man 2 write, respectively) to read the input file and write to the output file. Additionally, your program should only output runs of up to length 255, this is so the run-length specifier is guaranteed to always be one byte.

Run-length encoding is a compression technique that identifies "runs" of repeated characters and represents these compactly. The length of each run is counted, and the base is stored along with the number of repetitions of that basis. For example, the string AAABBBBBB consists of nine bytes, but it could be instead represented as 3A6B, where "A" and "B" are the basis of each run and the numbers give how many times each base is repeated.

The base does not need to be length-1, and this is what the third program parameter above specifies. For example, the string ABABABCDCDCDCD compresses very poorly with length-1 encoding to 1A1B1A1B1A1B1C1D1C1D1C1D1C1D, which is an expansion from 14 bytes to 28 bytes. However, if we allow our base to be length-2 then we can represent the above as 3AB4CD, which is a reduction from 14 bytes to 6 bytes.

Each run should be a maximum of 255 repetitions. This is so the run-length specifier can always be represented as a single byte (recall that an 8-bit unsigned integer can store values from 0-255). For example, if you had the character "A" stored 300 times in a row, then with a length-1 encoding your program should produce "255A45A" rather than "300A".

One can imagine many versions of RLE that are optimizations of the above principle. For this assignment I ask you to implement the simplest algorithm that mimics the behavior given above. In particular, given a run-length parameter K, then starting at the beginning of the input file:

- Read in the next K bytes of the file, and set a counter to one
- Scan the next K bytes of the file, if these bytes equal the first K bytes then increment the counter by one
- Repeat the previous step until a new pattern is found
- Output the value of the counter along with the original pattern
- Reset the counter to one and repeat the above process for the new pattern

Decompressing files is much easier:

- Read in the first byte of the file as an integer N
- Read in the next K bytes of the file as a length-K base pattern
- Output that pattern N times
- Repeat the above process
  
rle detects the following errors and quits gracefully:

- Not enough / too many command line arguments (and prints a usage statement)
- Compression length less than 1
- A mode value other than 0 or 1 (hint: use strtol())
- Input file does not exist or is otherwise not openable
- Any errors returned from open(), close(), read(), or write() - use the function perror() to print useful error messages

Upon encountering any error, print a useful message and exit() with a negative status code.

If no error is encountered then the program should not produce any output to standard output, and should only modify the output file.
