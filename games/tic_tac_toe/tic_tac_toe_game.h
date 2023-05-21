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
    TicTacToe(int pop_size, int games, bool pairs);

    int pop_size, games;
    float crossover_rate, mutation_rate;
    float avg_win_rate;

    vector<Individual> population;

    vector<Individual> first_population;
    vector<Individual> second_population;

    void CondensePairs();
    void MakePairs();

    void Generation();
    void GenerationPairs();

    // each individual plays out their current game
    void PlayGame();
    void PlayGamePairs();

    void PrintPopStats();
    
    void FreeVectors();

    // give a fitness val to each individual based on win/loss/ties
    void EvaluateFitness(vector<Individual> &population);

    void SetGeneticParams(float crossover_rate, float mutation_rate);
};


#endif