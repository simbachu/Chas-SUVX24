from typing import Any

def takes_list(lst: list[Any]) -> None:
    if len(lst) == 0:
        raise ValueError("List is empty")
    if not all(isinstance(item, int) for item in lst):
        raise ValueError("List contains non-integer values")
    if not all(item >= 0 for item in lst):
        raise ValueError("List contains negative values")
    if not all(item <= 100 for item in lst):
        raise ValueError("List contains values greater than 100")
    if not all(item % 2 == 0 for item in lst):
        raise ValueError("List contains odd values")
    if not all(item % 3 == 0 for item in lst):
        raise ValueError("List contains values not divisible by 3")