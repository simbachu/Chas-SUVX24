import math
def isprime(n):
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
    if n < 0:
        return None
    if n in _fib_mem:
        return _fib_mem[n]
    else:
        res = fibbonacci(n-1) + fibbonacci(n-2)
        _fib_mem[n] = res
        return fibbonacci(n-1) + fibbonacci(n-2)
def combination(n, k):
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
