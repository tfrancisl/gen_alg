#ifndef CHROMOSOME_CPP_
#define CHROMOSOME_CPP_

#include "chromosome.h"

template<int gene_len, int gene_count>
Chromosome<gene_len, gene_count>::Chromosome(){
    this->bits = bitset<gene_len*gene_count>(0);
    this->fitness = 1.0f;
    this->length = gene_len*gene_count;
}

template<int gene_len, int gene_count>
Chromosome<gene_len, gene_count>::Chromosome(bitset<gene_len*gene_count> b, float f, int l) {
    this->bits = b;
    this->fitness = f;
    this->length = l;
}

template<int gene_len, int gene_count>
void Chromosome<gene_len, gene_count>::PrintChromosome() {
    std::cout << this->bits.to_string() << std::endl;
}

template <int gene_len, int gene_count>
void Chromosome<gene_len, gene_count>::ChromosomeToFile(string fn) {
    ofstream f;

	f.open(fn, std::ios::out);

    f << this->bits.to_string() << std::endl;
    
    f.close();
}

template <int gene_len, int gene_count>
void Chromosome<gene_len, gene_count>::ChromosomeFromFile(string str) {

    this->bits = bitset<gene_count*gene_len>(str);
    
}

#endif