#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

/*
 * Alex Kalu
 * An Algorithm for Computing Quotient and Remainder Polynomials
 *
 * TODO:
 * What are the requirements on the Iterators and the ValueTypes?
 * Compare with other algorithms
 */
template <typename I0, typename I1>
I0 long_division(I0 f0, I0 l0, I1 f1)
{
    using T = typename std::iterator_traits<I0>::value_type;

    T a_n = *f1++;
    I0 current = f0;
    T b_n = *f0++;

    while (f0 != l0) {
        *current = (a_n * *f0 - b_n * *f1) / a_n;
        ++current;
        ++f0;
        ++f1;
    }

    return current;
}

template <typename I0, typename I1>
I0 long_division(I0 f0, I0 l0, I1 f1, I1 l1)
{
    using T = typename std::iterator_traits<I0>::value_type;
    I0 current = f0;
    T a_n = *f1++;
    T b_n = *f0++;
    while (f1 != l1) {
        *current = (a_n * *f0 - b_n * *f1) / a_n;
        ++current;
        ++f0;
        ++f1;
    }
    return std::copy(f0, l0, current);
}

int main()
{
    std::vector<int> a { 2, 1, 0, 0, -6, 9 };
    std::vector<int> b { 1, -3, 1 };
    auto last = a.end();
    for (int i = 0; i < 4; ++i) {
        last = long_division(a.begin(), last, b.begin(), b.end());
        std::copy(a.begin(), last, std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;
    }
}
