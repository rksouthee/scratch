#include <cmath>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>

template <typename I = int>
class zipfian_distribution {
private:
    static constexpr double euler_gamma = 0.577215664901532;
    std::uniform_real_distribution<double> dist;
    double ln_n;

public:
    using result_type = I;

    explicit zipfian_distribution(I n) :
        ln_n(log(double(n))) { }

    template <typename E>
    result_type operator()(E& e)
    {
        double r = dist(e);
        return result_type(floor(exp(r * ln_n + euler_gamma * (r - 1))));
    }

    static constexpr result_type min()
    {
        return result_type(0);
    }

    static constexpr result_type max()
    {
        return std::numeric_limits<result_type>::max();
    }
};

int main()
{
    std::default_random_engine e{2988};
    zipfian_distribution<int> d{10};
    std::vector<int> values(100000);
    std::generate(values.begin(), values.end(), [&] () { return d(e); });
    /* std::copy(values.begin(), values.end(), */
    /*           std::ostream_iterator<int>(std::cout, " ")); */
    for (int i = 0; i < 10; ++i) {
        std::cout << i << ": " << std::count(values.begin(), values.end(), i) << '\n';
    }
}
