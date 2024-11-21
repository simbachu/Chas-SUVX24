#ifndef CHAS_SCOREKEEPER_H
#define CHAS_SCOREKEEPER_H

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

class ScoreKeeper {
    private:
    std::fstream f;
    int entries();

    public:
    ScoreKeeper(std::string);

    void save_score(std::string name, short score);
    void high_scores();
    bool eligible(short score);
};

#endif