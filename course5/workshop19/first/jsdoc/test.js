/**
 * Calculates the total price of an item including tax
 * @param {number} _price - The price of the item
 * @param {number} _quantity - The quantity of the item
 * @param {number} _taxRate - The tax rate as a decimal
 * @returns {number} The total price of the item including tax
 */
function calculateTotal(_price, _quantity, _taxRate)
{
return _price * _quantity * (1 + _taxRate)
}

/**
 * Represents a user of the system
 * @param {string} _name - The name of the user
 * @param {string} _email - The email of the user
 */
class User
{
    constructor(_name, _email)
    {
        this.name = _name
        this.email = _email
    }
}