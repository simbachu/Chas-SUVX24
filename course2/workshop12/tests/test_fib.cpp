#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../fib.cpp"

TEST_CASE("Fibonnaci numbers"){
 // N * (0.694) ~ bits
 // N = 46 should be break point for 32 bits
 // N = 93 for unsigned long long/u64

    uint16_t input_1 {1};
    uint16_t input_2 {2};
    uint16_t input_3 {3};
    uint32_t input_4 {4};
    uint32_t input_5 {5};
    uint32_t input_46 {46};
    uint64_t input_50 {50};
    uint64_t input_93 {93};

    CHECK( fib(input_1) == 1);
    CHECK( fib(input_2) == 1);
    CHECK( fib(input_3) == 2);
    CHECK( fib(input_4) == 3);
    CHECK( fib(input_5) == 5);
    CHECK( fib(input_46) == 1836311903);
    CHECK( fib(input_50) == 12586269025 );
    CHECK( fib(input_93) == 12200160415121876738ULL);
}