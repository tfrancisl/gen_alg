#include "tic_tac_toe_game.h"
#include "time.h"
#include <iostream>


int main(void) {
    int gens = 1000;
    int games = 250;

    srand((int)time(NULL));

    TicTacToe *ttt = new TicTacToe(10);

    ttt->SetGeneticParams(0.7,0.001);

    for (int i=0; i<gens; i++) {
        ttt->Generation(games);

        //ttt->PrintPopStats();
    }

    return 0;
}