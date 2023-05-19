#include "tic_tac_toe_game.h"
#include "time.h"
#include <iostream>


int main(void) {
    int gens = 100;
    int games = 100;
    int pop_size = 24;

    srand(1337);

    TicTacToe *ttt = new TicTacToe(pop_size, games);

    ttt->SetGeneticParams(0.75,0.001);

    for (int i=0; i<gens; i++) {
        ttt->GenerationPairs();
    }

    return 0;
}