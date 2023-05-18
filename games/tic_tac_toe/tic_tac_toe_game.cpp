#include "tic_tac_toe_game.h"
#include <algorithm>
#include <iterator>

TicTacToe::TicTacToe() {}

TicTacToe::TicTacToe(int pop_size) {
    this->pop_size = pop_size;
    this->population.assign(this->pop_size, Individual() );

}

void TicTacToe::PlayGame() {

    for (int i=0; i<this->pop_size; i++) {

    }

}

void TicTacToe::EvaluateFitness() {

}

// make new individual with random genes, etc
Individual::Individual() {

}

// make new individual with given genes


// after each move made, will need to evaluate the board
void Individual::MakeMove() {
    bitset<18> current_state;


}

void Individual::RandomPlayer() {

}


// return -> -1 = loss, 0 = tie, 1 = win, otherwise keep going
int Individual::EvaluateBoard() {
    //hard code this... for ease of use?
    vector<array<int, 3>> sol_idxs = {{0,1,2},
                                      {3,4,5},
                                      {6,7,8},
                                      {0,3,6},
                                      {1,4,7},
                                      {2,5,8},
                                      {0,4,8},
                                      {2,4,6}};

    array<int, 8> sol_counts = {0,0,0,0,0,0,0,0};

    // loop through 8 possible solutions
    for (int i=0; i<8; i++) {
        sol_counts[i] += this->current_game[sol_idxs[i][0]];
        sol_counts[i] += this->current_game[sol_idxs[i][1]];
        sol_counts[i] += this->current_game[sol_idxs[i][2]];
    }

    auto loser = std::find(sol_counts.begin(), sol_counts.end(), -3);
    auto winner = std::find(sol_counts.begin(), sol_counts.end(), 3);
    auto empty = std::find(this->current_game.begin(), this->current_game.end(), 0);

    //doesn't properly handle error case of finding winner and loser
    if(loser != std::end(sol_counts) && winner == std::end(sol_counts) ) {  //definitive loser
        return -1;
    } else if (loser == std::end(sol_counts) && winner != std::end(sol_counts) ) {  //definitive winner
        return 1;
    } else if (empty != std::end(this->current_game)) { //tie game
        return 0;
    } else {    //game should keep going
        return 1000;
    }

}
