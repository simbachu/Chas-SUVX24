def take_any(options):
    """
    Returns one of the options, if you are indecisive
    Note:
      Your CPU branch predictor will love you for this
    Parameters:
      options: list of any
    Returns:
      any
    Example:
      --> take_any([1, 2, 3])
        2
      --> take_any(['a', 'b', 'c'])
        'b'
      --> take_any([True, False])
        True
    """
    import random

    return random.choice(options)

def ascending(a, b):
    """
    # Sort function that returns True if a should come before b (ascending order)
    # Parameters:
    #   a: first element
    #   b: second element
    # Returns:
    #   bool: True if a <= b, False otherwise
    """
    return a <= b

def descending(a, b):
    """
    Sort function that returns True if a should come before b (descending order)
    Parameters:
      a: first element
      b: second element
    Returns:
      bool: True if a >= b, False otherwise
    """
    return a >= b

def fun_sort(list, additional_sort_functions=[ascending, descending, take_any]):
    """
    Can't pick a way to sort a list?
    Let's take any.
    Uses a list of sort function, then takes any from the list.
    By default, it picks between acending, descending and iterated take_any.
    Parameters:
      list: list of any
      additional_sort_function: function that takes two arguments and returns a boolean
    Returns:
      list
    Example:
      --> fun_sort([3, 1, 4, 1, 5])
        [1, 1, 3, 4, 5]  # or [5, 4, 3, 1, 1] or some random arrangement
    """
    # Choose a random sort function
    chosen_sort = take_any(additional_sort_functions)

    # Apply the chosen sort function
    if chosen_sort == take_any:
        # For take_any, just return the list in random order
        import random
        result = list.copy()
        random.shuffle(result)
        return result
    else:
        # For ascending/descending, use the comparison function
        result = list.copy()
        # Simple bubble sort implementation using the comparison function
        n = len(result)
        for i in range(n):
            for j in range(0, n - i - 1):
                if not chosen_sort(result[j], result[j + 1]):
                    result[j], result[j + 1] = result[j + 1], result[j]
        return result