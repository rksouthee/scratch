#include <iostream>
#include <functional>
#include <tuple>
#include <utility>

template <typename I, typename N, typename P>
std::pair<I, N> find_if_n(I f, N n, P p)
{
    while (n != N(0) && !p(*f)) {
        ++f;
        --n;
    }
    return std::make_pair(f, n);
}

template <typename I, typename N, typename R>
std::pair<I, N> find_adjacent_mismatch_nonempty_n(I f, N n, R r)
{
    I t;
    do {
        t = f;
        ++f;
        --n;
    } while (n != N(0) && r(*t, *f));
    return std::make_pair(f, n);
}

template <typename I, typename N, typename P, typename R>
std::tuple<I, I, N> search_consecutive_if_n(I f, N n, N m, P p, R r)
{
    if (n == N(0)) return std::make_tuple(f, f, n);
    while (true) {
        std::pair<I, N> x = ::find_if_n(f, n, p);
        if (x.second < m) return std::make_tuple(x.first, x.first, x.second);
        std::pair<I, N> y = find_adjacent_mismatch_nonempty_n(x.first, m, r);
        if (y.second == N(0)) return std::make_tuple(x.first, y.first, x.second - m);
        f = y.first;
        n = x.second - (m - y.second);
    }
}

int main()
{
    int a[] = { 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1 };
    /* int a[] = { 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1 }; */
    std::size_t n = sizeof(a) / sizeof(int);
    std::size_t m = 7;
    const auto p = [] (int x) -> bool { return x == 0 || x == 1; };
    const auto r = std::equal_to<int>{};
    const auto t = search_consecutive_if_n(a, n, m, p, r);
    std::cout << (std::get<1>(t) - std::get<0>(t)) << '\n';
    std::cout <<  std::get<2>(t) << '\n';
}
