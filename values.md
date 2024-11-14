# Expressions and their values

## Overview

An __expression__ is some computation based on __operators__ and __operands__.


## xvalue

An __expiring__ result of an expression. The data associated with this value can be reused and new data can be written in the address in which it existed.

__xvalue__ can be either __rvalue__ or __glvalue__.

## glvalue

The __g__ stands for __generalized__. __lvalue__ is always a glvalue, and __xvalue__ is glvalue unless it is __rvalue__.

A glvalue is distinguised by having an __identifier__, like a variable or a function. lvalues are results of __assign__ operations.

## prvalue

A __prvalue__ can't be assigned to, and will not last past the expression that creates it.

´´´ C++
// prvalue expressions
2;     // literal
2 + 2; // arithmetic operator result
2 < 3; // comparision
fn();  // return value from a function (unless return value is reference)
´´´




## rvalue

__rvalue__ can be either __prvalue__ or __xvalue__.


