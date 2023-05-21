#include "tic_tac_toe_game.h"
#include "time.h"
#include <iostream>

int main(void) {
    int gens = 50;
    int games = 250;
    int pop_size = 50;

    srand(13337);

    TicTacToe *ttt = new TicTacToe(pop_size, games, true);

    ttt->SetGeneticParams(0.75,0.001);

    std::cout << gens << " generations with " << games << " games in " << pop_size/2 << " pairs." << std::endl;
    for (int i=0; i<gens; i++) {
        ttt->GenerationPairs();
    }

    ttt->CondensePairs();

    std::cout << gens << " generations with " << games << " games as " << pop_size << " independents against random players." << std::endl;
    for (int i=0; i<gens; i++) {
        ttt->Generation();
    }

    ttt->MakePairs();

    std::cout << gens << " generations with " << games << " games in " << pop_size/2 << " pairs." << std::endl;
    for (int i=0; i<gens; i++) {
        ttt->GenerationPairs();
    }

    ttt->CondensePairs();

    std::cout << gens << " generations with " << games << " games as " << pop_size << " independents against random players." << std::endl;
    for (int i=0; i<gens; i++) {
        ttt->Generation();
    }

    ttt->MakePairs();

    std::cout << gens << " generations with " << games << " games in " << pop_size/2 << " pairs." << std::endl;
    for (int i=0; i<gens; i++) {
        ttt->GenerationPairs();
    }

    ttt->FreeVectors();

    return 0;
}