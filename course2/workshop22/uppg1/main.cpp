#include <iostream>
#include <sstream>
#include <memory>
int main() {
// Static memory allocation
    int players {3};
    
    int max_scores[players]{};


// Dynamic memory allocation

    int *player_score_array = new int[players]();

// User input

    std::string line;
    for ( auto player = 0; player < players; player++ ){
        std::cout << "Player " << player+1 << ": ";
        std::getline(std::cin, line);
        std::stringstream input (line);
        int score {};
        input >> score;

        player_score_array[player] = score;
        if ( player_score_array[player] > max_scores[player] ){
            max_scores[player = score];
        }
    }

// Display scores
    for ( auto player = 0; player < players; player++ ){
        std::cout << "Player " << player+1 << " scored " << player_score_array[player] << " (record: " << max_scores[player] << ")" << '\n';
    }
// Free dynamically allocated memory

    delete[](player_score_array);
}