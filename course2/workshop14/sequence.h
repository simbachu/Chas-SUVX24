#ifndef CHAS_COLLECTIONS_SEQ_H
#define CHAS_COLLECTIONS_SEQ_H

#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <typeinfo>
#include <algorithm>

template <class Container>
class sequence{
    public: 
    Container seq;
    sequence(Container in) : seq{in}{

    }
    void show(){

        std::cout << "*** Sequence: " << typeid(seq).name() << " ***" << '\n';

        // Access elements
        auto it = seq.begin();
        auto max = std::max_element(seq.begin(), seq.end());
        std::cout << "First element: " << *it << '\n';
        std::cout << "Maximum element: " << *max << '\n';

        // Iterate using for-each loop
        for (int d : seq) {
            std::cout << d << " ";
        }
        std::cout << '\n';



        // Size of the container
        std::cout << "Size: " << seq.size() << '\n';
    }
};

#endif