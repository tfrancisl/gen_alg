#ifndef CHROMOSOME_H_
#define CHROMOSOME_H_
#include <string>
#include <map>
#include <iostream>
#include <math.h>
#include <bitset>
#include <iostream>
#include <fstream>
#include <vector>

using std::string;
using std::vector;
using std::bitset;
using std::array;
using std::ofstream;

template<int gene_len, int gene_count>
class Chromosome {

public:
    bitset<gene_len*gene_count> bits;
    int length;
    float fitness;  

    Chromosome();
    //Chromosome(const Chromosome<gene_len, gene_count> &c);
    Chromosome(bitset<gene_len*gene_count> b, float f, int l);

    void PrintChromosome();
    void ChromosomeToFile(string fn);

    void ChromosomeFromFile(string fn);
};

#include "chromosome.cpp"

#endif //CHROMOSOME_H_