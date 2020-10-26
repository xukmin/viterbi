Convolutional Encoder and Viterbi Decoder
=========================================

Author: Min Xu &lt;xukmin@gmail.com&gt;

This program implements both a Convolutional Encoder and a Viterbi Decoder in C++.

Running ViterbiCodec
--------------------

Compile:

```bash
make
```

Run:

```bash
./viterbi_main <constraint> <polynomial>... <bits>
```

Example use:

Decoding:

```bash
./viterbi_main 3 7 5 0011100001100111111000101100111011
010111001010001
```

Encoding:

```bash
$ ./viterbi_main 3 7 5 --encode 010111001010001
0011100001100111111000101100111011
```

Please note there are `(constraint - 1)` "0" bits padded in front of the original message.

More Features
-------------

- It supports reading input from stdin or from commandline arguments. When
  reading from stdin, it ignores blank lines or comment lines (which start with
  `#`).

- It supports any number of generator polynomials.

- It can perform convolutional encoding by providing `--encode` commandline
  flag.

- It supports a different notation of generator polynomials by providing
  `--reverse_polynomials` commandline flag.

Here are more options to run the program.

Show help message:

```bash
./viterbi_main --help
```

Read input from stdin:

```bash
./viterbi_main [--reverse_polynomials] [--encode]
```

Input format is:

```
<constraint> <polynomial>... <bits>
```

Read input from commandline arguments:

```bash
./viterbi_main [--reverse_polynomials] [--encode] <constraint> <polynomial>... <bits>
```

Flags:

```bash
--reverse_polynomials
    Reverse polynomials. E.g. 6 (=0b110) becomes 3 (=0b011).

--encode
    Do encoding instead of decoding.
```

Example usage:

```bash
./viterbi_main 3 7 5 0011100001100111111000101100111011
./viterbi_main 3 6 5 111011011100101011
./viterbi_main 7 91 121 1010110100001101001000011101
./viterbi_main 7 91 117 121 111100101110001011110101111111001011100111
./viterbi_main --reverse_polynomials 3 3 5 111011011100101011
./viterbi_main --encode 3 7 5 010111001010001
./viterbi_main --encode 3 6 5 1001101
./viterbi_main --encode --reverse_polynomials 3 3 5 1001101
```

Error Handling
--------------

All inputs are validated, and proper error messages will be output.

- The `constraint` should be greater than `0`.
- A generator polynomial should be greater than `0` and less than
  `1 << constraint`.
- The received bit sequence should be consisted with only `0` and `1`.
- The received bit sequence should be of length `N * <num-of-polynomials>` where
  `N` is an integer. Otherwise some bits must be missing during transmission.
  We will fill in appropriate number of trailing zeros.

Running Unit Test
-----------------

Run unit test:

```bash
make test
```

Or

```bash
make viterbi_test && ./viterbi_test
```

If the test passes, it will output `PASSED` and the end of the output;
otherwise, the test program aborts with assertion error.

There are a few sample cases in the unit test, some are with manually injected
bit errors.

It also generates random messages of length `8`, `16` and `32` for several different
convolutional codes and test if they can be properly encoded and decoded.

Dependencies
------------

The code is self-contained, meaning it depends on nothing but the C++ standard
library. The purpose is to make it easy to be integrated in any project.

