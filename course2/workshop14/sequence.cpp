#include "sequence.h"
#include <iostream>

std::ostream& operator<< (std::ostream& o, std::pair<const int, int> p){
    return o << p.first << ", " << p.second;
}