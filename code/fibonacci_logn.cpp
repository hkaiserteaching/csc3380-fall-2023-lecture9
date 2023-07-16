#include <concepts>
#include <functional>
#include <iostream>
#include <type_traits>

// check whether a given type is regular
template <typename T>
concept Regular = std::regular<T>;

template <typename T>
concept Integer = std::integral<T>;

// check whether the domain of a given operation is the same as its argument
template <typename Op, typename A>
concept Domain = std::same_as<std::invoke_result_t<Op, A, A>, A>;

// we can't check whether Op is associative, assume true
template <typename Op>
concept SemigroupOperation = true;

template <Integer N>
bool odd(N n)
{
    return bool(n & 0x1);
}

template <Integer N>
N half(N n)
{
    return n >> 1;
}

template <Regular A, Integer N, SemigroupOperation Op>
    requires(Domain<Op, A>)
A power_accumulate_semigroup(A r, A a, N n, Op op)
{
    // precondition: n >= 0
    if (n == 0)
        return r;
    while (true)
    {
        if (odd(n))
        {
            r = op(r, a);
            if (n == 1)
                return r;
        }
        n = half(n);
        a = op(a, a);
    }
}

template <Regular A, Integer N, SemigroupOperation Op>
    requires(Domain<Op, A>)
A power_semigroup(A a, N n, Op op)
{
    // precondition: n > 0
    while (!odd(n))
    {
        a = op(a, a);
        n = half(n);
    }
    if (n == 1)
        return a;
    return power_accumulate_semigroup(a, op(a, a), half(n - 1), op);
}

struct matrix
{
    int x11, x12;
    int x21, x22;

    friend bool operator==(matrix const& lhs, matrix const& rhs)
    {
        return lhs.x11 == rhs.x11 && lhs.x12 == rhs.x12 && lhs.x21 == rhs.x21 &&
            lhs.x22 == rhs.x22;
    }

    friend bool operator!=(matrix const& lhs, matrix const& rhs)
    {
        return !(lhs == rhs);
    }

    friend matrix operator*(matrix const& lhs, matrix const& rhs)
    {
        return matrix{lhs.x11 * rhs.x11 + lhs.x12 * rhs.x21,
            lhs.x11 * rhs.x12 + lhs.x12 * rhs.x22,
            lhs.x21 * rhs.x11 + lhs.x22 * rhs.x21,
            lhs.x21 * rhs.x12 + lhs.x22 * rhs.x22};
    }
};

int fib(int n)
{
    if (n < 2)
        return n;

    auto result =
        power_semigroup(matrix{1, 1, 1, 0}, n - 2, std::multiplies<matrix>());

    return result.x11;
}

int main()
{
    int num;
    std::cout << "Enter the number : ";
    std::cin >> num;
    std::cout << "\nThe fibonacci number : " << fib(num) << '\n';
    return 0;
}
