#ifndef TIC_TAC_TOE_H_
#define TIC_TAC_TOE_H_

#include <bitset>
#include <vector>
#include <array>

using std::vector;
using std::bitset;
using std::array;

#define GENE_LENGTH     4       // 4 bits to encode which of the 9 grids we'll play in
#define GENE_COUNT      2<<9    // 3 states (N, X, O) for 9 grid squares
#define CHROMO_LENGTH   GENE_COUNT*GENE_LENGTH

//class Individual;

class TicTacToe {

public:
    TicTacToe();
    TicTacToe(int pop_size);
    ~TicTacToe();

    int pop_size;

    vector<Individual> population;

    // each individual plays out their current game
    void PlayGame();

    // random player makes a move on the given game
    //void RandomPlayer(array<int, 9> &game);
    
    // give a fitness val to each individual based on win/loss/ties
    void EvaluateFitness();
};

class Individual { 

public:
    Individual();
    ~Individual();

    bitset<CHROMO_LENGTH> genes;
    int wins, losses, ties;
    bool first;         // goes first or second?
    float fitness;

    // 0 = None, -1 = X, 1 = O  ->  could be different type
    array<int, 9> current_game;

    // Make a move on your current game
    void MakeMove();

    // Random player makes a move on your current game
    void RandomPlayer();

    int EvaluateBoard();

};

#endif