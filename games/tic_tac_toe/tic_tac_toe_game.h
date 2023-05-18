#ifndef TIC_TAC_TOE_H_
#define TIC_TAC_TOE_H_

#include <bitset>
#include <vector>
#include <array>

using std::vector;
using std::bitset;
using std::array;

#define GENE_LENGTH     4       // 4 bits to encode which of the 9 grids we'll play in
#define GENE_COUNT      (1<<(18))    // 2 bits to represent 3 states, for each of the 9 grids
#define CHROMO_LENGTH   GENE_COUNT*GENE_LENGTH

//returns a float between 0 & 1
#define RANDOM_NUM			((float)rand()/((float)(RAND_MAX)+1))

//returns a random integer between 0 and N-1
#define RANDOM_NUM_RANGE(N)	rand() % (int)(N)

class Individual;

class TicTacToe {

public:
    TicTacToe();
    TicTacToe(int pop_size);
    ~TicTacToe();

    int pop_size;

    vector<Individual> population;

    // each individual plays out their current game
    void PlayGame();

    void PrintPopStats();
    
    // give a fitness val to each individual based on win/loss/ties
    void EvaluateFitness();
};

class Individual { 

public:
    Individual();
    //~Individual();

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

    void ClearBoard();

};

#endif