#include "tic_tac_toe_game.h"
#include "time.h"
#include <iostream>
#include <sstream>
#include <fstream>

int main(void) {
    int gens = 50;
    int games = 1000;
    int pop_size = 50;

    srand(6969);

    TicTacToe *ttt = new TicTacToe(pop_size, games, true);

    ttt->SetGeneticParams(0.75,0.001);

    std::ifstream f("old_fittest.bits");
    std::stringstream buf;

    buf << f.rdbuf();

    std::cout << gens << " generations with " << games << " games in " << pop_size/2 << " pairs." << std::endl;
    ttt->SeedPopulations(10, buf.str(), true);
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
    ttt->fittest.ChromosomeToFile("fittest.bits");
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