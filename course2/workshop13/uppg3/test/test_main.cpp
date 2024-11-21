#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/Secret_Number.h"
#include "../src/ScoreKeeper.h"

SCENARIO("Guessing that the secret number 1 is 1"){
    GIVEN("1 was chosen as the secret number"){
        Secret_Number s(1);
        REQUIRE(s.spoiler() == 1);
    
        WHEN("The guess is 1"){
            short our_guess {1};
            
            THEN ("We get an equal result to our guess back"){
                    REQUIRE(s.compare(our_guess) == our_guess);
            }
        }
    }
}

SCENARIO("Guessing that the secret number 1 is 2"){
    GIVEN("1 was chosen as the secret number"){
        Secret_Number s(1);
        REQUIRE(s.spoiler() == 1);
    
        WHEN("The guess is 2"){
            short our_guess {2};
            
            THEN ("We get a smaller result to our guess back"){
                    REQUIRE(s.compare(our_guess) < our_guess);
            }
        }
    }
}

SCENARIO("Guessing that the secret number 50 is 2"){
    GIVEN("50 was chosen as the secret number"){
        Secret_Number s(50);
        REQUIRE(s.spoiler() == 50);
    
        WHEN("The guess is 2"){
            short our_guess {2};
            
            THEN ("We get a greater result to our guess back"){
                REQUIRE(s.compare(our_guess) > our_guess);
            }
        }
    }
}

SCENARIO("Reading a high score"){
    GIVEN("The file with highscore.txt is empty"){
        auto hs = []{
            try {
                ScoreKeeper hs("highscore.txt");
                return hs;
            }
            catch(...){
                throw;
            }
        }();

        WHEN("We read the empty file"){
            THEN ("Exception"){
                CHECK_THROWS(hs.high_scores());
            }
        }
    }
}