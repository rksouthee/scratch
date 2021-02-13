# Scratch

These are various programs I've written for fun or learning.

## Tom's Data Onion

This was my attempt at Tom's Data Onion programming puzzle. More information
can be found in the directory `toms_data_onion`.

## Binomial Queue

`binomial_queue.cpp`

This is an implementation of a [Binomial Queue](https://en.wikipedia.org/wiki/Binomial_heap) using
an interesting data structure. I first saw this data structure in a lecture given by Alex Stepanov
where he mentioned that a binomial queue could be implemented with this technique.

The program requires the code from [Elements of Programming](http://elementsofprogramming.com/).

## EWD924

`ewd924.cpp`

I watched a video of a lecture given by [Edsger
Dijkstra](https://en.wikipedia.org/wiki/Edsger_W._Dijkstra) where he
demonstrated a beautiful algorithm for computing both the greatest common
divisor and lowest common multiple. I went to learn more about the algorithm
and found [EWD924](https://www.cs.utexas.edu/users/EWD/transcriptions/EWD09xx/EWD924.html)
where Dijkstra mentions the Game of Stanley Gill. I wrote an article,
[The Game of Stanley Gill](https://rksouthee.github.io/2019/06/28/the-game-of-stanley-gill.html)
that proved the correctness of the algorithm.

## Memory Arena

`memory_arena.cpp`

The `memory_arena` is an allocator that allocates in blocks, but doesn't free it's memory until the end.
This is useful in scenarios when building up some data structure that can then be released all at once.
The C++ Standard Library introduced `std::pmr::monotonic_buffer_resource` in C++17 that solves the same
problem as `memory_arena`.

## Polynomial Long Division

`polynomial_long_division.cpp`

This is an implementation of an algorithm described in [An Algorithm for Computing Quotient and Remainder Polynomials](https://www.semanticscholar.org/paper/An-Algorithm-For-Computing-Quotient-And-Remainder-Kalu/f82301d6945cbee6b4f2aa812fd068a838d3fe7d).

## Regular

`regular.cpp`

I like thinking about how languages can support Generic Programming succinctly. These notes are
one idea about a possible API for achieving regular functions generically.

## Search for `n` Consecutive Values

`search_n.cpp`

I was helping somebody with a problem they had, and come up with this solution.
I like doing programs like these because they're examples of how the Standard
Template Library can be extended with user defined algorithms.

## Sudoku Solver

`sudoku_solver.cpp`

This was a simple example of backtracking algorithms. This could be expanded by
providing a user interface that allows the input of an arbitrary sudoku puzzle
and generate a solution(s).

## Thue-Morse Sequence

`thue_morse.cpp`

I was helping somebody generate the [Thue-Morse
sequence](https://en.wikipedia.org/wiki/Thue%E2%80%93Morse_sequence).  The
problem was to implement two ways of generating the sequence.
