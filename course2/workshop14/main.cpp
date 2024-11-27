#include "sequence.h"

int main(){
    sequence<std::array<int, 5>> arr_int {std::array<int, 5> {1, 2, 3, 4, 5}};
    sequence<std::vector<int>> vec_int {std::vector<int> {2, 4, 6, 8, 10}};
    sequence<std::list<int>> list_int {std::list<int> {1, 3, 5, 7, 9}};
    sequence<std::set<int>> set_int {std::set<int> {1, 1, 2, 3, 5}};
    sequence<std::map<int, int>> map_int {std::map<int, int> {{1,4}, {2,8}, {3,16}}};


    arr_int.show();
    vec_int.show();
    list_int.show();
    set_int.show();
    //map_int.show();

}