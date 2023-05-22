#include "tic_tac_toe_game.h"
#include "../../bitset_genetics.h"
#include "individual.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>

using std::string;
using std::ofstream;

TicTacToe::TicTacToe() {}

TicTacToe::TicTacToe(int pop_size, int games, bool pairs) {
    this->pop_size = pop_size;
    this->games = games;

    if (pairs) {
        this->first_population.assign(this->pop_size/2, Individual());
        this->second_population.assign(this->pop_size/2, Individual());
    } else {
        this->population.assign(this->pop_size, Individual());
    }

}

void TicTacToe::FreeVectors() {
    this->population.clear();
    this->first_population.clear();
    this->second_population.clear();
}

void TicTacToe::CondensePairs() {

    this->population = this->first_population;

    for (int i = 0; i<this->second_population.size(); i++) {
        this->population.push_back(this->second_population[i]);
    }

    this->first_population.clear();
    this->second_population.clear();
}

void TicTacToe::MakePairs() {

    for (int i = 0; i<this->pop_size/2; i++) {
        this->first_population.push_back( this->population[i] );
        this->second_population.push_back( this->population[i+ (this->pop_size)/2] );
    }


}

void TicTacToe::Generation() {
    float total_fitness = 0;
    int total_wins = 0;
    int total_ties = 0;

    for (int i=0; i<this->games; i++) {
        this->PlayGame();
    }

    this->EvaluateFitness(this->population);

    for (int i=0; i<this->pop_size; i++) {
        total_fitness += this->population[i].genes.fitness;
        total_wins += this->population[i].wins;
        total_ties += this->population[i].ties;
    }
    this->avg_win_rate = (float)total_wins/(float)(this->games*this->pop_size);
    this->total_fitness = total_fitness;


    std::cout << "Average fitness: " << total_fitness/(float)this->pop_size << std::endl;
    std::cout << "Average winrate: " << this->avg_win_rate << std::endl;
    std::cout << "Average tie rate: " << (float)total_ties/(float)(this->games*this->pop_size) << std::endl;

}

void TicTacToe::GenerationPairs() {

    float total_fitness = 0.f;

    for (int i=0; i<this->games; i++) {
        this->PlayGamePairs();
        std::swap(this->first_population, this->second_population);
        if (i%2==0) std::rotate(this->second_population.begin(), this->second_population.begin()+2, this->second_population.end());
    }

    this->EvaluateFitness(this->first_population);
    this->EvaluateFitness(this->second_population);

    for (int i=0; i<this->pop_size/2; i++) {
        total_fitness += this->first_population[i].genes.fitness;
        total_fitness += this->second_population[i].genes.fitness;
    }

    this->total_fitness = total_fitness;

}

// use these routines to calculate stats other than total_fitness (for csv mostly)
void TicTacToe::CalculateStats() {
    int total_wins = 0, total_ties = 0;

    for (int i=0; i<this->pop_size/2; i++) {
        total_wins += this->population[i].wins;
        total_ties += this->population[i].ties;
    }

    this->avg_win_rate = (float)total_wins/(float)(this->games*this->pop_size);
    this->avg_tie_rate = (float)total_ties/(float)(this->games*this->pop_size);
}

void TicTacToe::CalculateStatsPairs() {
    int total_wins = 0, total_ties = 0;

    for (int i=0; i<this->pop_size/2; i++) {
        total_wins += this->first_population[i].wins;
        total_wins += this->second_population[i].wins;
        total_ties += this->first_population[i].ties;
        total_ties += this->second_population[i].ties;
    }

    this->avg_win_rate = (float)total_wins/(float)(this->games*this->pop_size);
    this->avg_tie_rate = (float)total_ties/(float)(this->games*this->pop_size);

    this->tie_rates.push_back(avg_tie_rate);
    this->win_rates.push_back(avg_win_rate);
    this->fitness_avg.push_back(this->total_fitness/this->pop_size);
}

void TicTacToe::PrintCSV(string fn) {
    ofstream f;

	f.open(fn, std::ios::out);

    f << "generation,avg_fitness,avg_win,avg_tie\n";

    for (int i = 0; i<fitness_avg.size(); i++) {
        f << i << "," << fitness_avg[i] << "," \
          << win_rates[i] << "," \
          << tie_rates[i] << "\n";
    }
    f.close();

}


void TicTacToe::GeneticRoutine() {
    vector<Chromosome<GENE_LENGTH, GENE_COUNT>> tmp_pop;

    for (int i=0; i<(this->pop_size); i++) {
        tmp_pop.push_back(Chromosome<GENE_LENGTH,GENE_COUNT>(this->population[i].genes));
    }

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

void TicTacToe::GeneticRoutinePairs() {
    vector<Chromosome<GENE_LENGTH, GENE_COUNT>> tmp_pop;

    for (int i=0; i<(this->pop_size)/2; i++) {
        tmp_pop.push_back(Chromosome<GENE_LENGTH,GENE_COUNT>(this->first_population[i].genes));
        tmp_pop.push_back(Chromosome<GENE_LENGTH,GENE_COUNT>(this->second_population[i].genes));
    }

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

    for (int i=0; i<population.size(); i++) {
        population[i].genes.fitness = ((float)population[i].wins)/2.0f + ((float)population[i].ties);
        //population[i].genes.fitness = ((float)population[i].wins);
    }

}

void TicTacToe::SetGeneticParams(float crossover_rate, float mutation_rate) {
    this->crossover_rate = crossover_rate;
    this->mutation_rate = mutation_rate;
}
