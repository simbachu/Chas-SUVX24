#include "Secret_Number.h"

Secret_Number::Secret_Number() : gen{std::random_device{}()}, dist{1,100}, n_{dist(gen)}{
}

Secret_Number::Secret_Number(short n) : gen{std::random_device{}()}, dist{1,100}, n_{n}{
}

void Secret_Number::regen(){
    n_ = dist(gen);
}

short Secret_Number::compare(short guess){
    short comp {0};

    comp += guess < n_;
    comp -= guess > n_;
    
    return guess + comp;
}

short Secret_Number::spoiler(){
    return n_;
}