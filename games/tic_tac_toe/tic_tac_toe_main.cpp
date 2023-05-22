#include "tic_tac_toe_game.h"
#include "time.h"
#include <iostream>

int main(void) {
    int gens = 500;
    int games = 1000;
    int pop_size = 50;

    srand(13337);

    TicTacToe *ttt = new TicTacToe(pop_size, games, true);

    ttt->SetGeneticParams(0.75,0.001);

    std::cout << gens << " generations with " << games << " games in " << pop_size/2 << " pairs." << std::endl;
    for (int i=0; i<gens; i++) {
        ttt->GenerationPairs();
        ttt->CalculateStatsPairs();
        ttt->GeneticRoutinePairs();
    }
    ttt->PrintCSV("test0_test.csv");

    std::cout << "\nagainst randoms after self training\n";
    ttt->CondensePairs();
    ttt->Generation();
    ttt->MakePairs();

    ttt->fitness_avg.clear();
    ttt->win_rates.clear();
    ttt->tie_rates.clear();

    std::cout << gens << " generations with " << games << " games in " << pop_size/2 << " pairs." << std::endl;
    for (int i=0; i<gens; i++) {
        ttt->GenerationPairs();
        ttt->CalculateStatsPairs();
        ttt->GeneticRoutinePairs();
    }
    ttt->PrintCSV("test1_test.csv");

    std::cout << "\nagainst randoms after self training\n";
    ttt->CondensePairs();
    ttt->Generation();

    /*ttt->CondensePairs();

    std::cout << gens << " generations with " << games << " games as " << pop_size << " independents against random players." << std::endl;
    for (int i=0; i<gens; i++) {
        ttt->Generation();
        ttt->GeneticRoutine();
    }

    ttt->MakePairs();

    std::cout << gens << " generations with " << games << " games in " << pop_size/2 << " pairs." << std::endl;
    for (int i=0; i<gens; i++) {
        ttt->GenerationPairs();
        ttt->GeneticRoutinePairs();
    }*/

    ttt->FreeVectors();

    return 0;
}