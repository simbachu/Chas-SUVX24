import math
def isprime(n):
    '''! Determine if a number is prime.
    @param n t int
    @retval t bool True if n is prime, otherwise False
    @throws None if n is an int; TypeError for non-int types due to modulo
    @pre n should be an integer; boundary: n <= 1 is not prime
    @code{.py}
    isprime(2)      # True
    isprime(15)     # False
    isprime(-7)     # False
    @endcode
    @bug Does not explicitly validate type; non-integer n may raise TypeError
    '''
    if n <= 1:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    p = 3
    while p*p <= n:
        if n % p == 0:
            return False
        p += 2
    return True
def nextprime(n):
    '''! Return the next prime strictly greater than n (or the first prime for small n).
    @param n t int
    @retval t int The next prime number
    @throws RecursionError for very large n due to recursion depth
    @pre n should be an integer; boundary: n <= 1 returns 2; n == 2 returns 3
    @code{.py}
    nextprime(2)    # 3
    nextprime(3)    # 5
    nextprime(14)   # 17
    @endcode
    @bug No type check; non-integer n may lead to TypeError or incorrect behavior
    '''
    if n <= 1:
        return 2
    if n == 2:
        return 3
    if n % 2 == 0:
        return nextprime(n-1)
    if isprime(n+2):
        return n+2
    return nextprime(n+2)
def primefactors(n):
    '''! Compute the prime factorization of n.
    @param n t int
    @retval t list List of prime factors in non-decreasing order
    @throws None for n >= 1; may loop unexpectedly for invalid types
    @pre n should be an integer >= 1; boundary: n == 1 returns []
    @code{.py}
    primefactors(1)     # []
    primefactors(12)    # [2, 2, 3]
    primefactors(97)    # [97]
    @endcode
    @bug Negative or zero values are not validated; n <= 0 returns [] without error
    '''
    factors = []
    p = 2
    while n > 1:
        if n % p == 0:
            factors.append(p)
            n = n // p
        else:
            p = nextprime(p)
    return factors
def power(c, n):
    '''! Compute c raised to the power n using fast exponentiation for integer n.
    @param c t number Base (int, float, or any type supporting * and pow)
    @param n t int|float Exponent; integers use fast recursion, non-ints use math.pow
    @retval t number Result of c**n
    @throws RecursionError for very large |n| when n is int; TypeError if operations unsupported
    @pre Prefer integer n for exact arithmetic; boundary: n == 0 returns 1, n < 0 returns reciprocal
    @code{.py}
    power(2, 10)     # 1024
    power(2, -3)     # 0.125
    power(2.0, 0.5)  # 1.4142135623730951 (uses math.pow)
    @endcode
    @bug No explicit type checks; non-integer n uses math.pow which returns float and may lose precision
    '''
    if type(n) != int:
        return math.pow(c,n)
    elif n < 0:
        return 1/power(c, -n)
    elif n == 0:
        return 1
    elif n % 2 == 0:
        return power(c*c, n//2)
    else:
        return c * power(c, n-1)
_fact_mem = {0: 1}
def factorial(n):
    '''! Compute n! using memoized recursion.
    @param n t int
    @retval t int|None n! for n >= 0; None for n < 0
    @throws RecursionError for very large n; KeyError not expected due to memoization
    @pre n should be a non-negative integer; boundary: n == 0 returns 1
    @code{.py}
    factorial(0)   # 1
    factorial(5)   # 120
    factorial(-1)  # None
    @endcode
    @bug No type validation; non-integer n will raise TypeError in comparisons or recursion
    '''
    if n < 0:
        return None
    elif n in _fact_mem:
        return _fact_mem[n]
    else:
        res = n*factorial(n-1)
        _fact_mem[n] = res
        return res
_fib_mem = {1: 0, 2: 1}
def fibbonacci(n):
    '''! Compute the nth Fibonacci number using memoization (1-indexed: F1=0, F2=1).
    @param n t int
    @retval t int|None The nth Fibonacci number; None for n < 0
    @throws RecursionError if called with n == 0 due to missing base case; deep recursion otherwise
    @pre n should be an integer >= 1; boundary: n == 1 -> 0, n == 2 -> 1
    @code{.py}
    fibbonacci(1)  # 0
    fibbonacci(6)  # 5
    fibbonacci(-2) # None
    @endcode
    @bug n == 0 is not handled and leads to recursion into negative indices
    '''
    if n < 0:
        return None
    if n in _fib_mem:
        return _fib_mem[n]
    else:
        res = fibbonacci(n-1) + fibbonacci(n-2)
        _fib_mem[n] = res
        return fibbonacci(n-1) + fibbonacci(n-2)
def combination(n, k):
    '''! Compute the binomial coefficient C(n, k) using recursive relations.
    @param n t int Total number of items
    @param k t int Number of chosen items
    @retval t int|None C(n, k) for valid 0 <= k <= n; None otherwise
    @throws RecursionError for large n, k due to deep recursion
    @pre Integers n >= 0, k >= 0, and 0 <= k <= n; boundary: k == 0 or n == k returns 1
    @code{.py}
    combination(5, 2)   # 10
    combination(10, 0)  # 1
    combination(6, 6)   # 1
    @endcode
    @bug Does not validate inputs; out-of-bound values may return None or behave unexpectedly
    '''
    if n == k or k == 0:
        return 1
    elif k > 0:
        return (combination(n, k-1)*(n-k+1)) // k
    elif k < n:
        return (combination(n-1, k)*n) // (n - k)
    elif n > 0 and k > 0:
        return (combination(n-1, k-1)*n) // k
    else:
        return None
