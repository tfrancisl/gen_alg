#include "tic_tac_toe_game.h"
#include "time.h"
#include <iostream>


int main(void) {
    int games = 10;

    srand((int)time(NULL));

    TicTacToe *ttt = new TicTacToe(1);


    for (int i=0; i<games; i++) {
    std::cout << "Game " << i << "\n";
        ttt->PlayGame();
    }
    std::cout << "*****\n";

    ttt->PrintPopStats();

    return 0;
}