#include "ScoreKeeper.h"

ScoreKeeper::ScoreKeeper(std::string filename) : f {filename}{
    f.exceptions(std::fstream::failbit | std::fstream::badbit);
}

int ScoreKeeper::entries(){
    return std::count_if(std::istreambuf_iterator<char>{f}, {}, [](char c) { return c == '\n'; });
}

void ScoreKeeper::save_score(std::string name, short score){
    f.seekg(0);
    f << name << " " << score << '\n';
}

void ScoreKeeper::high_scores(){
    std::string out {};
    f.seekp(0);
    for ( int record = 1; record <= entries(); record++ ){
        try{
            f >> out;
        }
        catch(...) {
            throw;
        }
        std::cout << record << ": " << out;
    }

}