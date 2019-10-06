#include <cstdlib>
#include <memory>
#include <vector>

class memory_arena {
public:
    static constexpr size_t block_size = 1024;
    static constexpr size_t max_align = alignof(max_align_t);

private:
    void* _ptr = nullptr;
    size_t _space = 0;
    std::vector<void*> _blocks;

public:
    ~memory_arena()
    {
        for (void* p : _blocks)
            free(p);
    }

    void* allocate(size_t bytes, size_t alignment = max_align)
    {
        // Precondition: is_power_of_two(alignment)
        if (!std::align(alignment, bytes, _ptr, _space)) {
            _space = std::max(bytes + alignment, block_size);
            _ptr = malloc(_space);
            _blocks.push_back(_ptr);
            std::align(alignment, bytes, _ptr, _space);
        }
        void* ptr = _ptr;
        _ptr = static_cast<unsigned char*>(_ptr) + bytes;
        _space -= bytes;
        return ptr;
    }

    size_t space() const
    {
        return _space;
    }
};

constexpr size_t memory_arena::block_size;
constexpr size_t memory_arena::max_align;

#include <cstring>
#include <iostream>
#include <iterator>
#include <numeric>

int main()
{
    memory_arena arena;
    int* p0 = static_cast<int*>(arena.allocate(sizeof(int) * 10));
    std::iota(p0, p0 + 10, 0);
    std::copy(p0, p0 + 10, std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
    void* p1 = arena.allocate(1000);
    if (!p1) {
        std::cerr << "error allocating memory\n";
        return EXIT_FAILURE;
    }
    char* name = static_cast<char*>(p1);
    strcpy(name, "Robert Southee");
    std::cout << name << '\n';
}
