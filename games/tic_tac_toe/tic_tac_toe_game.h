#ifndef TIC_TAC_TOE_H_
#define TIC_TAC_TOE_H_

#include <bitset>
#include <vector>
#include <array>
#include "chromosome.h"

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
    TicTacToe(int pop_size, int games);

    int pop_size, games;
    float crossover_rate, mutation_rate;
    float avg_win_rate;

    vector<Individual> population;

    vector<Individual> first_population;
    vector<Individual> second_population;

    void Generation();
    void GenerationPairs();

    // each individual plays out their current game
    void PlayGame();
    void PlayGamePairs();

    void PrintPopStats();

    void EvaluateFitness(vector<Individual> &population);

    // give a fitness val to each individual based on win/loss/ties
    void EvaluateFitness();

    void SetGeneticParams(float crossover_rate, float mutation_rate);
};

class Individual { 

public:
    Individual();
    Individual(Chromosome<GENE_LENGTH, GENE_COUNT> genes);

    //bitset<CHROMO_LENGTH> genes;
    Chromosome<GENE_LENGTH, GENE_COUNT> genes;
    int wins, losses, ties;
    bool first;         // goes first or second?

    // 0 = None, -1 = X, 1 = O  ->  could be different type
    array<int, 9> current_game;

    // Make a move on your current game
    void MakeMove();
    void MakeMovePairs(bitset<CHROMO_LENGTH> genes, bool first);

    // Random player makes a move on your current game
    void RandomPlayer();

    int EvaluateBoard();

    void ClearBoard();

};

#endif