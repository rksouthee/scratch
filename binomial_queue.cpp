#include "eop-code/eop.h"
#include "eop-code/print.h"

template <typename C, typename R>
    requires(BifurcateCoordinate(C) && Relation(R))
struct binomial_combine {
    relation_source<C, C, R> rs;

    binomial_combine(const R& r) : rs(r) { }

    C operator()(C x, C y)
    {
        if (rs(y, x)) {
            set_right_successor(x, left_successor(y));
            set_left_successor(y, x);
            return y;
        } else {
            set_right_successor(y, left_successor(x));
            set_left_successor(x, y);
            return x;
        }
    }
};

template <typename C, typename R>
struct input_type<binomial_combine<C, R>, 0> {
    typedef C type;
};

template <typename I, typename R>
struct min_source {
    relation_source<I, I, R> rs;
    min_source(const R& r) : rs(r) { }
    const I& operator()(const I& x, const I& y)
    {
        if (rs(y, x)) return y;
        return x;
    }
};

template <typename I, typename R>
struct input_type<min_source<I, R>, 0> {
    typedef I type;
};

template <typename R>
struct binomial_queue {
    typedef Domain(R) T;
    typedef stree_coordinate<T> C;
    int dummy = 0;
    R r;
    C z;
    C f[64];
    DistanceType(pointer(C)) n;
    binomial_combine<C, R> op;

    binomial_queue() :
        r(), z(0), n(0), op(r) { }

    binomial_queue(const R& r) :
        r(r), z(0), n(0), op(r) { }

    ~binomial_queue()
    {
        typedef DistanceType(pointer(C)) N;
        for (N i(0); i < n; ++i)
            bifurcate_erase(f[i], stree_node_destroy<T>());
    }

    DistanceType(pointer(C)) size() const
    {
        typedef DistanceType(pointer(C)) N;
        N s(0);
        for (N i(0); i < n; ++i) {
            if (!empty(f[i])) s = s + (1 << i);
        }
        return s;
    }

    void push(const T& x)
    {
        C tmp = add_to_counter(f, f + n, op, stree_node_construct<T>()(x), z);
        if (tmp != z) {
            sink(f + n) = tmp;
            n = successor(n);
        }
    }

    const T& top() const
    {
        min_source<C, R> m_op(r);
        return source(reduce_nonzeroes(f, f + n, m_op, z));
    }

    void pop()
    {
        min_source<C, R> m_op(r);
        C c = reduce_nonzeroes(f, f + n, m_op, z);
        pointer(C) i = find(f, f + n, c);
        c = left_successor(c);
        stree_node_destroy<T>()(*i);
        sink(i) = z;
        while (c != z) {
            --i;
            C tmp = right_successor(c);
            set_right_successor(c, z);
            add_to_counter(i, f + n, op, c, z);
            c = tmp;
        }
    }
};

int main()
{
    binomial_queue<less<int>> queue;
    queue.push(12);
    queue.push(18);
    queue.push(1);
    queue.push(10);
    queue.push(5);
    print(queue.size());
    print_eol();
    for (int i = 0; i < 5; ++i) {
        print(queue.top());
        print_eol();
        queue.pop();
    }
    print(queue.size());
    print_eol();
}
