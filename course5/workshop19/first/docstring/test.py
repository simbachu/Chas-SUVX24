def calculate_compound_interest(_principal, _rate, _time, _compound_frequency):
    """
    Calculates the compound interest of an investment
    """
    return _principal * (1 + _rate/_compound_frequency) ** (_compound_frequency * _time)

class BankAccount:
    """
    Represents a bank account
    """
    def __init__(self, _account_holder, _balance=0):
        """
        Initializes a bank account

        Args:
            _account_holder: The name of the account holder
            _balance: The initial balance of the account (default is 0)
        """
        self.account_holder = _account_holder
        self.balance = _balance

    def deposit(self, _amount):
        """
        Deposits an amount into the bank account

        Args:
            _amount (float): The amount to deposit

        Raises: ValueError: if the amount is negative
        """
        if _amount < 0:
            raise ValueError("Can't deposit a negative amount")
        self.balance += _amount

    def withdraw(self, _amount):
        """
        Withdraws an amount from the bank account

        Args:
            _amount (float): The amount to withdraw

        Raises: ValueError: if the amount is negative
        """
        if _amount < 0:
            raise ValueError("Can't withdraw a negative amount")
        self.balance -= _amount

    def get_balance(self):
        """
        The balance of the bank account

        Returns:
            float: The balance of the bank account
        """
        return self.balance

