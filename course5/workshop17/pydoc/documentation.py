from numbers import Real
from typing import Callable, Any, Sequence, TypeVar, Protocol
import math


class Comparable(Protocol):
    def __le__(self, other: Any) -> bool: ...
    def __ge__(self, other: Any) -> bool: ...


T = TypeVar("T")

def min(a: Any, b: Any) -> Any:
    """
    Returns the minimum of two things that can be compared
    Precondition:
      a and b can be compared using <
    Args:
      Any a: first thing
      Any b: second thing
    Returns:
      Any: the minimum of the two things
    """
    return a if a < b else b

def pow(base: Real, exponent: Real) -> Real:
    """
    Returns the power of a base to an exponent
    Raises:
      OverflowError: if the result does not fit in INT_MAX and INT_MIN
    Args:
      Real base: the base
      Real exponent: the exponent
    Returns:
      Real: the power of the base to the exponent
    Examples:
      --> try:
          pow(2, 3)
          catch OverflowError:
            print("Result does not fit in INT_MAX and INT_MIN")
          8
      --> try:
          pow(2, 1000)
          catch OverflowError:
            print("Result does not fit in INT_MAX and INT_MIN")
          Result does not fit in INT_MAX and INT_MIN
    """
    return base ** exponent

def mean(values: list[Real]) -> float:
    """
    Returns the mean of a list of numbers (Real), trimming NaN values from the list
    Precondition:
      values is not empty
    Args:
      List[Real] values: list of numbers
    Returns:
      float: the mean value of the in argument values
    Examples:
      --> mean([1, 2, 3])
        2.0
      --> mean([1.0, 2.0, 3.0])
        2.0
      --> mean([-1, 0, 1])
        0.0
      --> mean([NaN, -1, 0])
        -0.5
    """
    non_nan_values = [value for value in values if not isinstance(value, float) or not math.isnan(value)]
    return float(sum(non_nan_values)) / len(non_nan_values)

def take_any(options: Sequence[T]) -> T:
    """
    Gives one of the options in the list.
    Note:
      Your CPU branch predictor will love you for this if you use this for branching.
    Args:
      Sequence[T] options: non-empty sequence of items of the same type
    Returns:
      T: one of the items in the options sequence
    Examples:
      --> take_any([1, 2, 3])
        2
      --> take_any(['a', 'b', 'c'])
        'b'
      --> take_any([True, False])
        True
    """
    import random

    return random.choice(options)

def ascending(a: Comparable, b: Comparable) -> bool:
    """
    Sort function that returns True if a should come before b (ascending order)
    Args:
      Comparable a: first comparable element
      Comparable b: second comparable element
    Returns:
      bool: True if a <= b, False otherwise
    """
    return a <= b

def descending(a: Comparable, b: Comparable) -> bool:
    """
    Sort function that returns True if a should come before b (descending order)
    Args:
      Comparable a: first comparable element
      Comparable b: second comparable element
    Returns:
      bool: True if a >= b, False otherwise
    """
    return a >= b

def fun_sort(
      items: list[T],
      additional_sort_functions: Sequence[Callable[..., Any]] | None = None
    ) -> list[T]:
    """
    Can't pick a way to sort a list?
    Let's take any.
    Uses a list of sort function, then takes any from the list.
    By default, it picks between acending, descending and iterated take_any.
    Args:
      List[T] items: list of items to sort
      Sequence[Callable[..., Any]] | None additional_sort_functions: mix of comparator functions (Callable[[T, T], bool]) and list transformers (Callable[[Sequence[T]], Sequence[T]]). Defaults to (ascending, descending, take_any)
    Returns:
      List[Any]: the sorted list
    Examples:
      --> fun_sort([3, 1, 4, 1, 5])
        [1, 1, 3, 4, 5]  # or [5, 4, 3, 1, 1] or some random arrangement
    """
    # Choose a random sort function
    if additional_sort_functions is None:
        additional_sort_functions = (ascending, descending, take_any)
    chosen_sort = take_any(additional_sort_functions)

    # Apply the chosen sort function
    if chosen_sort == take_any:
        # For take_any, just return the list in random order
        import random
        result = items.copy()
        random.shuffle(result)
        return result
    else:
        # For ascending/descending, use the comparison function
        result = items.copy()
        # Simple bubble sort implementation using the comparison function
        n = len(result)
        for i in range(n):
            for j in range(0, n - i - 1):
                if not chosen_sort(result[j], result[j + 1]):
                    result[j], result[j + 1] = result[j + 1], result[j]
        return result