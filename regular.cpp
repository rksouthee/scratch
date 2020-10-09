
/*
 * What would it look like if the language allowed the programmer to
 * define procedures as opposed to the compiler generated versions.
 *
 * The following procedures are from Stepanov and McJones' book Elements
 * of Programming for Regular types.
 *
 * API
 *
 * size_t members_of(const type& t);
 *
 * returns the number of members/fields of a type e.g.
 * struct My_type {
 *  int x;
 *  int y;
 *  double z;
 *  std::string s;
 * };
 * static_assert(members_of(My_type) == 4);
 *
 *
 * const type& operator[](const type& t, size_t i);
 *
 * returns the i'th type of a composite type e.g.
 * struct My_type {
 *   int x;
 *   int y;
 *   double z;
 *   std::string s;
 * };
 * static_assert(My_type[0] == int);
 * static_assert(My_type[1] == int);
 * static_assert(My_type[2] == double);
 * static_assert(My_type[3] == std::string);
 *
 *
 * auto member(size_t i, const T& x) -> const T[i]&;
 * NOTE: i should be a constant expression
 *
 * returns an object of the i'th member of x: e.g.
 * struct My_type {
 *  int x;
 *  int y;
 *  double z;
 *  std::string s;
 * };
 * My_type value{29, 88, 3.14, "Robert"};
 * assert(member(0, value) == 29);
 * assert(member(1, value) == 88);
 * assert(member(2, value) == 3.14);
 * assert(member(3, value) == "Robert");
 * assert(member(4, value) == 0xbad); // Compile-time error
 *
 * The syntax is very idealistic, in a real programming language some
 * modifications would be necessary. I believe this could be a useful way
 * to describe generic procedures in a language since it allows to reuse
 * familiar elements from the language (as opposed to something like Template
 * Meta Programming).
 *
 * Using these basis procedures as an example it is easy to add more procedures
 * such as `hash_append`, `serialize`, `print`, etc.
 *
 * It is also possible to optimize some of the procedures to use intrinsics
 * such `memcpy`, `memset` and `memcmp` when suitable.
 */

// operator==(const T& x, const T& y);
template <typename T>
bool equal(const T& x, const T& y)
{
    for (int i = 0; i < members_of(T); ++i) {
        if (member(i, x) != member(i, y))
            return false;
    }
    return true;
}

// T::operator=(const T& y);
template <typename T>
void assign(T& x, const T& y)
{
    for (int i = 0; i < members_of(T); ++i)
        assign(member(i, x), member(i, y));
}

// T::~T();
template <typename T>
void destroy(const T& x)
{
    for (int i = members_of(T); i > 0; --i)
        destroy(member(i - 1, x));
}

// T::T();
template <typename T>
void construct(T& x)
{
    for (int i = 0; i < members_of(T); ++i)
        construct(member(i, x));
}

// T::T(const T& y);
template <typename T>
void construct(T& x, const T& y)
{
    for (int i = 0; i < members_of(T); ++i)
        construct(member(i, x), member(i, y));
}

// operator<(const T& x, const T& y);
template <typename T>
bool less(const T& x, const T& y)
{
    for (int i = 0; i < members_of(T); ++i) {
        if (member(i, x) < member(i, y))
            return true;
        if (member(i, y) < member(i, x))
            return false;
    }
    return false;
}

type underlying_type(const type& t)
{
    if (!is_composite(t))
        return t;
    type result(composite_type(members_of(t)));
    for (int i = 0; i < members_of(t); ++i)
        result[i] = underlying_type(t[i]);
    return result;
}
