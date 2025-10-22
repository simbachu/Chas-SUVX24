"""Usage reference and examples for `intmath.py` (Google style).

Overview:
    Concise usage notes for functions in `intmath.py`. Includes args, returns,
    potential exceptions to consider, and practical examples.

Functions
---------

isprime(n)
    Args:
        n (int): Candidate integer.
    Returns:
        bool: True if prime, else False.
    Raises:
        TypeError: If `n` is not int-like.

nextprime(n)
    Args:
        n (int): Lower bound.
    Returns:
        int: Smallest prime strictly greater than `n` (special cases for n<=1).
    Raises:
        RecursionError: For extremely large `n` due to recursion.
        TypeError: If inputs are not ints.

primefactors(n)
    Args:
        n (int): Integer to factor (n>1 recommended).
    Returns:
        list[int]: Prime factors with multiplicity. `n<=1` yields empty list.
    Raises:
        TypeError: If `n` is not an int.

power(c, n)
    Args:
        c (int|float): Base.
        n (Any): Exponent. If non-int, uses floating math.pow.
    Returns:
        int|float: c**n. Supports negative and zero integer exponents.
    Raises:
        TypeError: If types are unsupported by `math.pow` or integer recursion.
        ZeroDivisionError: When `c==0` and `n<0` (division by zero).

factorial(n)
    Args:
        n (int): Non-negative integer.
    Returns:
        int|None: n!; returns None for n<0.
    Raises:
        RecursionError: For very large n (~>950) due to recursion depth.
        TypeError: If `n` is not an int.

fibbonacci(n)
    Args:
        n (int): 1-indexed position (1->0, 2->1).
    Returns:
        int|None: n-th Fibonacci; None for n<0.
    Raises:
        RecursionError: For large n (~>950) due to recursion depth.
        TypeError: If `n` is not an int.

combination(n, k)
    Args:
        n (int): Number of items.
        k (int): Chosen items.
    Returns:
        int|None: Binomial coefficient; returns 1 for base cases. For invalid
            domains (e.g., k>n, negatives), code may return None.
    Raises:
        RecursionError: For large inputs due to deep recursion.
        TypeError: If `n`/`k` are not ints.
    BUG:
        Invalid domains (`n<0`, `k<0`, or `k>n`) are not validated in the
        current implementation; e.g., `combination(-1, 2)` returns an int
        instead of `None`.

Examples
--------

Basic usage:
    >>> from intmath import isprime, nextprime, primefactors, power, factorial, fibbonacci, combination
    >>> isprime(29)
    True
    >>> nextprime(29)
    31
    >>> primefactors(360)
    [2, 2, 2, 3, 3, 5]

    >>> power(2, 10)
    1024
    >>> round(power(2.0, 0.5), 10) == round(2.0 ** 0.5, 10)
    True

    >>> factorial(0)
    1
    >>> factorial(5)
    120

    >>> fibbonacci(1)
    0
    >>> fibbonacci(2)
    1

    >>> combination(5, 2)
    10

Handling invalid arguments that yield None (not exceptions):
    >>> from intmath import factorial, fibbonacci, combination
    >>> factorial(-1) is None
    True
    >>> fibbonacci(-3) is None
    True
    >>> combination(-1, 2) is None
    True

Catching potential exceptions (types, recursion, zero division):
    >>> from intmath import power, nextprime, factorial, fibbonacci, combination

    # Type errors for unsupported inputs
    >>> power("2", 10)  # doctest: +IGNORE_EXCEPTION_DETAIL
    Traceback (most recent call last):
    ...
    TypeError: ...

    # Zero division when base is zero and exponent negative
    >>> power(0, -1)
    Traceback (most recent call last):
    ...
    ZeroDivisionError: division by zero

    # Recursion risks for very large n (Python default limit ~1000)
    >>> factorial(2000)  # doctest: +SKIP
    Traceback (most recent call last):
    ...
    RecursionError: ...

    >>> fibbonacci(2000)  # doctest: +SKIP
    Traceback (most recent call last):
    ...
    RecursionError: ...

    >>> combination(2000, 1000)  # doctest: +SKIP
    Traceback (most recent call last):
    ...
    RecursionError: ...

    # nextprime rarely hits recursion limits, but catch defensively if using huge n
    >>> nextprime(10**12)  # doctest: +SKIP
    1000000000039

When to catch exceptions:
    - TypeError: User inputs or deserialized data may be non-numeric.
    - ZeroDivisionError: Guard power(0, negative); validate before calling.
    - RecursionError: For untrusted or very large n in factorial, fibbonacci,
      and combination (and rarely nextprime). Prefer iterative approaches or
      validate bounds pre-call.
"""

if __name__ == "__main__":
    import doctest
    doctest.testmod(verbose=True)


