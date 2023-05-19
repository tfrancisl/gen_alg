#include "tic_tac_toe_game.h"
#include "../../bitset_genetics.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>

using std::string;

TicTacToe::TicTacToe() {}

TicTacToe::TicTacToe(int pop_size, int games) {
    this->pop_size = pop_size;
    this->games = games;
    this->population.assign(this->pop_size, Individual());
    this->first_population.assign(this->pop_size/2, Individual());
    this->second_population.assign(this->pop_size/2, Individual());

}

void TicTacToe::Generation() {

    vector<Chromosome<GENE_LENGTH, GENE_COUNT>> tmp_pop;
    float total_fitness = 0;

    for (int i=0; i<this->games; i++) {
        this->PlayGame();
    }

    this->EvaluateFitness(this->population);
    //this->PrintPopStats();

    for (int i=0; i<this->pop_size; i++) {
        tmp_pop.push_back(Chromosome<GENE_LENGTH,GENE_COUNT>(this->population[i].genes));
        total_fitness += this->population[i].genes.fitness;
    }
    std::cout << "Average fitness: " << total_fitness/(float)this->pop_size << std::endl;
    std::cout << "Average winrate: " << this->avg_win_rate << std::endl;

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

void TicTacToe::GenerationPairs() {

    vector<Chromosome<GENE_LENGTH, GENE_COUNT>> tmp_pop;
    float total_fitness = 0.f;
    int total_wins = 0;

    for (int i=0; i<this->games; i++) {
        this->PlayGamePairs();
        std::rotate(this->second_population.begin(), this->second_population.begin()+1, this->second_population.end());
    }

    this->EvaluateFitness(this->first_population);
    this->EvaluateFitness(this->second_population);
    //this->PrintPopStats();

    for (int i=0; i<this->pop_size/2; i++) {
        tmp_pop.push_back(Chromosome<GENE_LENGTH,GENE_COUNT>(this->first_population[i].genes));
        tmp_pop.push_back(Chromosome<GENE_LENGTH,GENE_COUNT>(this->second_population[i].genes));
        total_fitness += this->first_population[i].genes.fitness;
        total_fitness += this->second_population[i].genes.fitness;
        total_wins += this->first_population[i].wins;
        total_wins += this->second_population[i].wins;
    }
    this->avg_win_rate = (float)total_wins/(float)(this->games*this->pop_size);

    std::cout << "Average fitness: " << total_fitness/(float)this->pop_size << std::endl;
    std::cout << "Average winrate: " << this->avg_win_rate << std::endl;

    this->first_population.clear();
    this->second_population.clear();

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
        this->first_population.push_back(Individual(new_chrm1));
        this->second_population.push_back(Individual(new_chrm2));

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
        } else {
            while (abs(res) > 1) {
                this->population[i].RandomPlayer();
                res = this->population[i].EvaluateBoard();
                if (abs(res) <= 1) break;

                this->population[i].MakeMove();
                res = this->population[i].EvaluateBoard();
            }
        }

        if (res == 0) {
            this->population[i].ties++;
        } else if (res == 1) {
            this->population[i].wins++;
        } else {
            this->population[i].losses++;
        }
        this->population[i].ClearBoard();

    }
}

void TicTacToe::PlayGamePairs()
{

    for (int i=0; i<this->pop_size/2; i++) {
        int res = 1000;

        while (abs(res) > 1) {
            this->first_population[i].MakeMovePairs(this->first_population[i].genes.bits, true);
            res = this->first_population[i].EvaluateBoard();
            if (abs(res) <= 1) break;

            this->first_population[i].MakeMovePairs(this->second_population[i].genes.bits, false);
            res = this->first_population[i].EvaluateBoard();        
        }

        if (res == 0) {
            this->first_population[i].ties++;
            this->second_population[i].ties++;
        } else if (res == 1) {
            this->first_population[i].wins++;
            this->second_population[i].losses++;
        } else {
            this->first_population[i].losses++;
            this->second_population[i].wins++;
        }
        this->first_population[i].ClearBoard();

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

void TicTacToe::EvaluateFitness(vector<Individual> &population) {
    //int total_wins = 0;

    for (int i=0; i<population.size(); i++) {
        population[i].genes.fitness = ((float)population[i].wins) + ((float)population[i].ties)/2.0f;
        //population[i].genes.fitness = ((float)population[i].wins);
        //population[i].genes.fitness = ((float)population[i].ties);
        //std::cout << "fitness " << population[i].genes.fitness << "\n";
        //total_wins += population[i].wins;
    }

    //this->avg_win_rate = (float)total_wins/(float)(this->games*this->pop_size);
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

void Individual::MakeMovePairs(bitset<CHROMO_LENGTH> genes, bool first) {
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
        action[i] = genes[GENE_LENGTH*(current_state.to_ulong()) + i];
    }

    int grid = action.to_ulong() % 9;   //mod it with 9 in case of overflow?

    if (this->current_game[grid] == 0) {
        if (first) {
            this->current_game[grid] = 1;
        } else {
            this->current_game[grid] = -1;
        }
    } else {
        bool move_taken = false;
        do {
        int r_sq = RANDOM_NUM_RANGE(9);
            if (this->current_game[r_sq] == 0) {
                if (first) {
                    this->current_game[r_sq] = 1;
                } else {
                    this->current_game[r_sq] = -1;
                }
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
