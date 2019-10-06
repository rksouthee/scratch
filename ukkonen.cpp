#include <algorithm>
#include <iterator>
#include <tuple>
#include <vector>
#include "rks/list_pool.h"

/*
 * Esko Ukkonen
 * On-Line Construction of Suffix Trees
 * 1995
 * https://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.10.751
 */
template <typename I>
// requires Readable(I) && RandomAccessIterator(I)
class suffix_tree {
private:
    using T = typename std::iterator_traits<I>::value_type;
    using N = typename std::iterator_traits<I>::difference_type;

    struct edge_t;
    using node_t = typename rks::list_pool<std::vector<edge_t>>::list_type;

    struct edge_t {
        T value;
        I first;
        I last;
        node_t child;

        edge_t() = default;

        edge_t(T v, I f, I l, node_t s) :
            value(v), first(f), last(l), child(s) { }
    };

    rks::list_pool<std::vector<edge_t>> _nodes;
    node_t _root;

    using edge_iterator = typename std::vector<edge_t>::iterator;

    std::vector<edge_t>& edges(node_t x)
    {
        return _nodes.value(x);
    }

    edge_iterator begin(node_t x)
    {
        return edges(x).begin();
    }

    edge_iterator end(node_t x)
    {
        return edges(x).end();
    }

    node_t& link(node_t x)
    {
        return _nodes.next(x);
    }

    std::pair<node_t, I> update(node_t s, I i, I f_i, I l_i)
    {
        node_t old_r = _root;
        const T& v = *f_i;
        while (true) {
            edge_iterator tmp = std::partition_point(
                begin(s), end(s), [&i] (const edge_t& x) {
                    return x.value > i[0];
                });
            if (i == f_i) {
                if (tmp != end(s) && tmp->value == v) break;
                edges(s).emplace(tmp, v, f_i, l_i, _nodes.end());
                if (old_r != _root) link(old_r) = s;
                old_r = s;
                s = link(s);
            } else {
                N n = f_i - i;
                I f_prime = tmp->first;
                I l_prime = tmp->last;
                node_t s_prime = tmp->child;
                const T& v_prime = f_prime[n];
                if (v == v_prime) break;
                node_t r = _nodes.allocate(std::vector<edge_t>(), _nodes.end());
                edges(r).emplace_back(v_prime, f_prime + n, l_prime, s_prime);
                tmp->last = f_prime + n;
                tmp->child = r;
                if (v > v_prime)
                    edges(r).emplace(begin(r), v, f_i, l_i, _nodes.end());
                else
                    edges(r).emplace_back(v, f_i, l_i, _nodes.end());
                if (old_r != _root) link(old_r) = r;
                old_r = r;
                std::tie(s, i) = canonize(link(s), i, f_i);
            }
        }
        if (old_r != _root) link(old_r) = s;
        return std::make_pair(s, i);
    }

    std::pair<node_t, I> canonize(node_t s, I i, I f_i)
    {
        do {
            edge_iterator tmp = std::partition_point(
                begin(s), end(s), [&i] (const edge_t& x) {
                    return x.value > i[0];
                });
            N n = tmp->last - tmp->first;
            if (n > f_i - i) break;
            s = tmp->child;
            i = i + n;
        } while (i != f_i);
        return std::make_pair(s, i);
    }

public:
    suffix_tree(I f_a, I l_a) :
        _root(_nodes.allocate(std::vector<edge_t>{}, _nodes.end()))
    {
        std::vector<edge_t> tmp;
        while (l_a != f_a) {
            tmp.emplace_back(*(l_a - 1), l_a - 1, l_a, _root);
            --l_a;
        }
        node_t bottom = _nodes.allocate(tmp, _nodes.end());
        link(_root) = bottom;
    }

    void operator()(I f_i, I l_i)
    {
        std::pair<node_t, I> p(_root, f_i);
        while (f_i != l_i) {
            p = update(p.first, p.second, f_i, l_i);
            ++f_i;
            p = canonize(p.first, p.second, f_i);
        }
    }

    template <typename O>
    O dfs(O f_o)
    {
        std::vector<std::pair<node_t, N>> stack;
        stack.emplace_back(_root, N(0));
        do {
            node_t s = stack.back().first;
            if (edges(s).empty()) {
                stack.pop_back();
            } else {
                edge_t tmp = edges(s).back();
                edges(s).pop_back();
                N n = stack.back().second + (tmp.last - tmp.first);
                if (_nodes.is_end(tmp.child)) *f_o++ = n;
                else stack.emplace_back(tmp.child, n);
            }
        } while (!stack.empty());
        return f_o;
    }
};

#include <iostream>
#include <numeric>
#include <string>

int main()
{
    using iterator = typename std::vector<short>::iterator;
    std::vector<short> ascii(257);
    std::iota(ascii.begin(), ascii.end(), 0);
    suffix_tree<iterator> st(ascii.begin(), ascii.end());

    std::string input("mississippi");
    std::vector<short> data(input.begin(), input.end());
    data.push_back(256);
    st(data.begin(), data.end());

    using N = typename std::iterator_traits<iterator>::difference_type;
    std::vector<N> sizes;
    st.dfs(std::back_inserter(sizes));

    std::copy(sizes.begin(), sizes.end(),
              std::ostream_iterator<N>(std::cout, " "));
    std::cout << '\n';
}
