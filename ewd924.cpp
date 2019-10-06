#include <cstdlib>
#include <iostream>
#include <utility>

/*
 * Edsger Dijkstra
 * On the cultural gap
 * The Game of Stanley Gill
 * https://www.cs.utexas.edu/users/EWD/transcriptions/EWD09xx/EWD924.html
 */
template <typename I>
// requires Integer(I)
std::pair<I, I> gill(const I& X, const I& Y)
{
    // Precondition: X > 0 && Y > 0
    I x = X; I y = Y;
    I u = X; I v = Y;
    while (x != y) {
        if (y < x) {
            x = x - y;
            u = u + v;
        } else {
            y = y - x;
            v = v + u;
        }
    }
    return { (x + y) / I(2),
             (u + v) / I(2) };
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "Usage: ewd924 X Y\n";
        return EXIT_FAILURE;
    }
    int X = atoi(argv[1]);
    int Y = atoi(argv[2]);
    std::pair<int, int> p = gill(X, Y);
    std::cout << "gcd(" << X << ", " << Y << ") = " << p.first << '\n';
    std::cout << "scm(" << X << ", " << Y << ") = " << p.second << '\n';
}
