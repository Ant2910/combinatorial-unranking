# combinatorial-unranking
## General
Generator for k-permutations and k-combinations in C++. These are represented by a simplified model based on an urn. It is based of the [draw-generator](https://github.com/iwi-se/draw-generator) and extends it by unranking-algorithms for all urn typs.

The implementations of the urn models are called: 
 * `UrnOR` – urn where the order is important and which contains repetitions.
 * `UrnO` – urn where the order is important and does not include repetitions.
 * `UrnR` – urn with unimportant order and with repetitions.
 * `Urn` – urn with unimportant order and without repetitions.

|                         | Order is important  | Order is not important      | 
| ----------------------- | ------------------- | --------------------------- |
| With repetition         | $`n^k`$             | $`(n+k−1)! \over k!(n−1)!`$ |
| Without repetition      | $`n! \over (n−k)!`$ | $`n! \over k!(n−k)!`$       |


An urn can then be created with `Urntype {n, k};`.
An example would be `UrnOR {3, 3};

## Theoretical foundations
For a deeper explanation of the different urn models and how the unranking-algorithmes work take a look at the included thesis.

## Usage and Compilation
To include the urn in the code use the include directive `#include "urn.hpp"`

Please note that the include path must be adjusted depending on the location of the source files.

In the sources files instructions for compilation is given. Some of the benchmarks need [Catch2](https://github.com/catchorg/Catch2).