# Scratch

These are various programs I've written for fun or learning.

## Tom's Data Onion

This was my attempt at Tom's Data Onion programming puzzle. More information
can be found in the directory `toms_data_onion`.

## Binomial Queue

This is an implementation of a [Binomial Queue](https://en.wikipedia.org/wiki/Binomial_heap) using
an interesting data structure. I first saw this data structure in a lecture given by Alex Stepanov
where he mentioned that a binomial queue could be implemented with this technique.

The program requires the code from [Elements of Programming](http://elementsofprogramming.com/).

## EWD924

I watched a video of a lecture given by [Edsger
Dijkstra](https://en.wikipedia.org/wiki/Edsger_W._Dijkstra) where he
demonstrated a beautiful algorithm for computing both the greatest common
divisor and lowest common multiple. I went to learn more about the algorithm
and found [EWD924](https://www.cs.utexas.edu/users/EWD/transcriptions/EWD09xx/EWD924.html)
where Dijkstra mentions the Game of Stanley Gill. I wrote an article,
[The Game of Stanley Gill](https://rksouthee.github.io/2019/06/28/the-game-of-stanley-gill.html)
that proved the correctness of the algorithm.

## Memory Arena

The `memory_arena` is an allocator that allocates in blocks, but doesn't free it's memory until the end.
This is useful in scenarios when building up some data structure that can then be released all at once.
The C++ Standard Library introduced `std::pmr::monotonic_buffer_resource` in C++17 that solves the same
problem as `memory_arena`.
