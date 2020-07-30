#include <iostream>
#include <iterator>
#include <utility>

template <typename I, typename N, typename O>
std::pair<I, O> copy_n(I f_i, N n, O f_o)
{
    while (n != N(0)) {
        *f_o++ = *f_i++;
        --n;
    }
    return std::make_pair(f_i, f_o);
}

class board {
private:
    int _board[9][9];

    bool is_in_row(int row, int val) const
    {
        for (int col = 0; col < 9; ++col)
            if (_board[row][col] == val) return true;
        return false;
    }

    bool is_in_col(int col, int val) const
    {
        for (int row = 0; row < 9; ++row)
            if (_board[row][col] == val) return true;
        return false;
    }

    bool is_in_block(int row, int col, int val)
    {
        row = row - row % 3;
        col = col - col % 3;

        for (int i = row; i < row + 3; ++i)
            for (int j = col; j < col + 3; ++j)
                if (_board[i][j] == val) return true;
        return false;
    }

    bool is_valid(int row, int col, int val)
    {
        return !is_in_row(row, val) &&
               !is_in_col(col, val) &&
               !is_in_block(row, col, val);
    }

public:
    explicit board(const int* values)
    {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                _board[i][j] = *values++;
    }

    int* copy(int* output) const
    {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                *output++ = _board[i][j];
        return output;
    }

    bool solve()
    {
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (_board[row][col] == 0) {
                    for (int val = 1; val <= 9; ++val) {
                        if (is_valid(row, col, val)) {
                            _board[row][col] = val;
                            if (solve()) {
                                return true;
                            } else {
                                _board[row][col] = 0;
                            }
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }
};

int main()
{
    int test_board[] = {
        8, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 3, 6, 0, 0, 0, 0, 0,
        0, 7, 0, 0, 9, 0, 2, 0, 0,
        0, 5, 0, 0, 0, 7, 0, 0, 0,
        0, 0, 0, 0, 4, 5, 7, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 3, 0,
        0, 0, 1, 0, 0, 0, 0, 6, 8,
        0, 0, 8, 5, 0, 0, 0, 1, 0,
        0, 9, 0, 0, 0, 0, 4, 0, 0,
    };

    board b(test_board);

    int solved[9 * 9];
    if (!b.solve()) {
        std::cout << "couldn't solve puzzle\n";
        return 0;
    }

    b.copy(solved);

    using I = const int*;
    using O = std::ostream_iterator<int>;
    std::pair<I, O> p(solved, O(std::cout, " "));
    for (int i = 0; i < 9; ++i) {
        p = ::copy_n(p.first, 9, p.second);
        std::cout << '\n';
    }
}
