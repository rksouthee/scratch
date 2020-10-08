#include <iostream>
#include <cstdlib>

int round_power_of_2(int n)
{
    int p = 1;
    while (p < n)
        p *= 2;
    return p;
}

bool* thue_generate1(int n)
{
    n = round_power_of_2(n);
    bool* thue = new bool[n];
    thue[0] = false;
    for (int i = 1; i < n; i *= 2) {
        for (int j = 0; j < i; ++j)
            thue[i + j] = !thue[j];
    }
    return thue;
}

bool* thue_generate0(int n)
{
    bool* thue = new bool[n];
    thue[0] = false;
    for (int i = 1; i < n; ++i) {
        if (i % 2 == 0)
            thue[i] = thue[i / 2];
        else
            thue[i] = !thue[i - 1];
    }
    return thue;
}

void visualize(const bool* thue, int n)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << "+-"[thue[i] ^ thue[j]] << "  ";
        }
        std::cout << '\n';
    }
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <n>\n";
        return -1;
    }

    int n = atoi(argv[1]);
    if (n == 0)
        return 0;

    bool* thue = thue_generate1(n);
    visualize(thue, n);

    delete[] thue;
}
