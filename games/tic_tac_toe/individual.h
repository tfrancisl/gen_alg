#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_
#include "tic_tac_toe_game.h"
#include "chromosome.h"

class Individual { 

public:
    Individual();
    //Individual(const Individual &i);
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