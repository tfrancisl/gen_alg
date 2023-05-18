#include "tic_tac_toe_game.h"
#include "../../bitset_genetics.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>

using std::string;

TicTacToe::TicTacToe() {}

TicTacToe::TicTacToe(int pop_size) {
    this->pop_size = pop_size;
    this->population.assign(this->pop_size, Individual() );

}

void TicTacToe::Generation(int games) {

    vector<Chromosome<GENE_LENGTH, GENE_COUNT>> tmp_pop;
    int total_fitness = 0;

    for (int i=0; i<games; i++) {
        this->PlayGame();
    }

    this->EvaluateFitness();
    //this->PrintPopStats();

    for (int i=0; i<this->pop_size; i++) {
        tmp_pop.push_back(Chromosome<GENE_LENGTH,GENE_COUNT>(this->population[i].genes));
        total_fitness += this->population[i].genes.fitness;
    }
    std::cout << "Average fitness: " << (float)total_fitness/(float)this->pop_size << "\n";

    this->population.clear();

    for (int i=0; i<(this->pop_size)/2; i++) {
        bitset<CHROMO_LENGTH> genes1,genes2;
	    Chromosome<GENE_LENGTH,GENE_COUNT> new_chrm1,new_chrm2;

        //Select two
        genes1 = Roulette<GENE_LENGTH, GENE_COUNT>(total_fitness, tmp_pop);
        genes2 = Roulette<GENE_LENGTH, GENE_COUNT>(total_fitness, tmp_pop);

        //crossover
        Crossover<GENE_LENGTH, GENE_COUNT>(genes1, genes2, this->crossover_rate);
        
        //mutate
        Mutate<GENE_LENGTH, GENE_COUNT>(genes1, this->mutation_rate);
        Mutate<GENE_LENGTH, GENE_COUNT>(genes2, this->mutation_rate);

        new_chrm1 = Chromosome<GENE_LENGTH,GENE_COUNT>(genes1, 0.f, CHROMO_LENGTH); 
        new_chrm2 = Chromosome<GENE_LENGTH,GENE_COUNT>(genes2, 0.f, CHROMO_LENGTH); 

        //set up the new individuals
        this->population.push_back(Individual(new_chrm1));
        this->population.push_back(Individual(new_chrm2));

    }

}



void TicTacToe::PlayGame()
{

    for (int i=0; i<this->pop_size; i++) {
        int res = 1000;

        if (this->population[i].first) {
            while (abs(res) > 1) {
                this->population[i].MakeMove();
                res = this->population[i].EvaluateBoard();
                if (abs(res) <= 1) break;

                this->population[i].RandomPlayer();
                res = this->population[i].EvaluateBoard();
            }

            if (res == 0) {
                this->population[i].ties++;
            } else if (res == 1) {
                this->population[i].wins++;
            } else {
                this->population[i].losses++;
            }
        }

        this->population[i].ClearBoard();

    }
}

void TicTacToe::PrintPopStats() {
    for (int i=0; i<this->pop_size; i++) {
        std::cout << "Individual " << i << "\n";
        std::cout << "Wins: " << this->population[i].wins << "\n";
        std::cout << "Losses: " << this->population[i].losses << "\n";
        std::cout << "Ties: " << this->population[i].ties << "\n";
        std::cout << "Fitness: " << this->population[i].genes.fitness << "\n";

    }
}

void TicTacToe::EvaluateFitness() {

    for (int i=0; i<this->pop_size; i++) {
        //for some reason I'm making fitness = 1/4 of wins, we'll see how this works
        this->population[i].genes.fitness = ((float)this->population[i].wins)/4.0f;
    }

}

void TicTacToe::SetGeneticParams(float crossover_rate, float mutation_rate) {
    this->crossover_rate = crossover_rate;
    this->mutation_rate = mutation_rate;
}

// make new individual with random genes, etc
Individual::Individual() {
    this->genes = Chromosome<GENE_LENGTH, GENE_COUNT>(bitset<CHROMO_LENGTH>(0), 0.f, CHROMO_LENGTH);

    for (int j=0; j<CHROMO_LENGTH; j++) {
        if (RANDOM_NUM < 0.5) {
            this->genes.bits[j].flip();
        }
    }

    this->wins = 0;
    this->losses = 0;
    this->ties = 0;
    this->first = true;
    this->current_game = {0,0,0,0,0,0,0,0,0};

}

Individual::Individual(Chromosome<GENE_LENGTH, GENE_COUNT> genes) {
    this->genes = genes;

    this->wins = 0;
    this->losses = 0;
    this->ties = 0;
    this->first = true;
    this->current_game = {0,0,0,0,0,0,0,0,0};
}

// after each move made, will need to evaluate the board
void Individual::MakeMove() {
    bitset<18> current_state;
    bitset<GENE_LENGTH> action;

    for (int i=0; i<9; i++) {
        int sq = this->current_game[i];
        if( sq == 0) {
            current_state[2*i+0] = 0;
            current_state[2*i+1] = 0;
        } else if ( sq == -1 ) {
            current_state[2*i+0] = 0;
            current_state[2*i+1] = 1;
        } else if ( sq == 1 ) {
            current_state[2*i+0] = 1;
            current_state[2*i+1] = 0;
        }
    }

    // get the action to take out of the genes using the current board state
    for (int i=0; i<GENE_LENGTH; i++) {
        action[i] = this->genes.bits[GENE_LENGTH*(current_state.to_ulong()) + i];
    }

    int grid = action.to_ulong() % 9;   //mod it with 9 in case of overflow?

    // set the chosen grid to 1, only if it's a valid move
    // this isn't quite right because if it's not a valid move, we won't take action... which is against tictactoe rules
    if (this->current_game[grid] == 0) {
        this->current_game[grid] = 1;
    } else {    //temporary solution: randomly choose if it doesn't work
                //really need a genetic method to handle this though
        bool move_taken = false;
        do {
        int r_sq = RANDOM_NUM_RANGE(9);
            if (this->current_game[r_sq] == 0) {
                this->current_game[r_sq] = 1;
                move_taken = true;
            }
        } while (!move_taken);
    }


}

void Individual::RandomPlayer() {
    bool move_taken = false;

    do {
        int r_sq = RANDOM_NUM_RANGE(9);

        if (this->current_game[r_sq] == 0) {
            this->current_game[r_sq] = -1;
            move_taken = true;
        }


    } while (!move_taken);

}


// return -> -1 = loss, 0 = tie, 1 = win, otherwise keep going
int Individual::EvaluateBoard() {
    //hard code this... for ease of use?
    vector<array<int, 3>> sol_idxs = {{0,1,2},
                                      {3,4,5},
                                      {6,7,8},
                                      {0,3,6},
                                      {1,4,7},
                                      {2,5,8},
                                      {0,4,8},
                                      {2,4,6}};

    array<int, 8> sol_counts = {0,0,0,0,0,0,0,0};

    /// print out board
    /*for (int i=0; i<9; i++) {
        string sym;
        int m = this->current_game[i];

        if (m == -1) sym = "X";
        else if (m == 0) sym = "-";
        else if (m == 1) sym = "O";

        std::cout << sym;
        if (i%3==2) std::cout << "\n";

    }
    std::cout << "\n";*/
    /// end

    // loop through 8 possible solutions
    for (int i=0; i<8; i++) {
        sol_counts[i] += this->current_game[sol_idxs[i][0]];
        sol_counts[i] += this->current_game[sol_idxs[i][1]];
        sol_counts[i] += this->current_game[sol_idxs[i][2]];
    }

    auto loser = std::find(sol_counts.begin(), sol_counts.end(), -3);
    auto winner = std::find(sol_counts.begin(), sol_counts.end(), 3);
    auto empty = std::find(this->current_game.begin(), this->current_game.end(), 0);

    //doesn't properly handle error case of finding winner and loser (would only happen if lines 191,192 dont work)
    if(loser != std::end(sol_counts) && winner == std::end(sol_counts) ) {  //definitive loser
        return -1;
    } else if (loser == std::end(sol_counts) && winner != std::end(sol_counts) ) {  //definitive winner
        return 1;
    } else if (empty == std::end(this->current_game)) { //tie game
        return 0;
    } else {    //game should keep going
        return 1000;
    }

}

void Individual::ClearBoard() {
    this->current_game = {0,0,0,0,0,0,0,0,0};
}
