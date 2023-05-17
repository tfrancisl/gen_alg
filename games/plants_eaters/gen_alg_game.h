#ifndef GEN_ALG_GAME_
#define GEN_ALG_GAME_

#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include "../../chromosome.h"
#include "entity.h"

#define STATE_SIZE                      4

//one bit for apex warning signal, 2 bits for what is directly in front of the eater, 2 bits for facing
#define EATER_TRAITS                    5         
#define EATER_GENE_COUNT                1<<(EATER_TRAITS+STATE_SIZE)
#define EATER_GENE_LENGTH               (2+STATE_SIZE)  //plus 2 for action
#define CHROMO_LENGTH				    (EATER_GENE_COUNT)*(EATER_GENE_LENGTH)

//4 bits for total eater count in facing grid, 2 bits for facing
#define APEX_TRAITS                     6
#define APEX_GENE_COUNT                 1<<(APEX_TRAITS+STATE_SIZE)
#define APEX_GENE_LENGTH               	(2+STATE_SIZE)  //plus 2 for action
#define APEX_CHROMO_LENGTH				(APEX_GENE_COUNT)*(APEX_GENE_LENGTH)

#define WORLD_SIZE 					40          // Cannot be moved to a variable since it is used for templating

#define MAX_DELETION_LENGTH         3*EATER_GENE_LENGTH

#define MAKE_CSV 1



using std::vector;
using std::ofstream;
class GenAlgGame {

public:
    GenAlgGame();
    GenAlgGame(int eater_pop_size, int plant_pop_size, int apex_pop_size);

    int time_step,eater_pop_size,plant_pop_size,apex_pop_size,gen;
    float crossover_rate, mutation_rate, deletion_rate;
    //ofstream csv_file;

    vector<float> max_eater_fitness;
    vector<float> max_apex_fitness;
    vector<float> avg_eater_fitness;
    vector<float> avg_apex_fitness;
    vector<float> plant_count;
    vector<float> gen_times;

    array<array<Entity, WORLD_SIZE>, WORLD_SIZE> *world;

    vector< Chromosome<EATER_GENE_LENGTH, EATER_GENE_COUNT> > eater_pop;
    vector< Chromosome<APEX_GENE_LENGTH, APEX_GENE_COUNT> > apex_pop;

    void SetGeneticParams(float crossover_rate, float mutation_rate, float deletion_rate);

    void ProgressTime();

    void Generation(int days);

    void RespawnPlantNearby(int x1, int y1);

    void GetRandomCoord(Entity ent);
    void GetRandomCoord(Entity ent, int r);

};

#endif //GEN_ALG_GAME_