/**
 * Returns one of the options, if you are indecisive
 * Note:
 *   Your CPU branch predictor will love you for this
 * @param {Array} options - The list of options to choose from.
 * @returns {any} The chosen option.
 */
function take_any(options){
    return options[Math.floor(Math.random() * options.length)];
}

/**
 * Sort function that returns True if a should come before b (ascending order)
 * @param {any} a - The first element.
 * @param {any} b - The second element.
 * @returns {boolean} True if a <= b, False otherwise.
 */
function ascending(a, b){
    return a <= b;
}

/**
 * Sort function that returns True if a should come before b (descending order)
 * @param {any} a - The first element.
 * @param {any} b - The second element.
 * @returns {boolean} True if a >= b, False otherwise.
 */
function descending(a, b){
    return a >= b;
}

/**
 * Can't pick a way to sort a list?
 * Let's take any.
 * Uses a list of sort function, then takes any from the list.
 * By default, it picks between acending, descending and iterated take_any.
 * @param {Array} list - The list to sort.
 * @param {Array} additional_sort_functions - The list of sort functions to choose from.
 * @returns {Array} The sorted list.
 * @throws {Error} If the sort function is not a function with the signature (a, b) => boolean.
 */
function fun_sort(list, additional_sort_functions=[ascending, descending, take_any]){
    const chosen_sort = take_any(additional_sort_functions);
    if(chosen_sort === take_any){
        return list.sort(() => Math.random() - 0.5);
    }
    try {
        return list.sort(chosen_sort);
    } catch (error) {
        console.error("Error sorting list:", error);
        return list;
    }
}
