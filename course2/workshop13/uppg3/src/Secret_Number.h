#ifndef CHAS_SECRET_NUMBER_H
#define CHAS_SECRET_NUMBER_H
#include <random>

class Secret_Number {
    private:
    std::mt19937 gen;
    std::uniform_int_distribution<short> dist;

    short n_;
    
    public:
    Secret_Number();
    Secret_Number(short); //set n_

    void regen();

    short compare(short);
    short spoiler();
};

#endif