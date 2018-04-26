#ifndef TYGR_POPULATION_HPP
#define TYGR_POPULATION_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include <omp.h>
#include <fstream>

#include "Innovation.hpp"
#include "GeneLink.hpp"
#include "GeneNode.hpp"
#include "Organism.hpp"
#include "GeneInfoController.hpp"


class GeneLink;
class GeneNode;
class Innovation;
class GeneInfoController;

class Population
{
 private:
    int genome_id;
    int species_idx;
    std::shared_ptr<Genome> generator_fully_connection_genome();
    std::shared_ptr<Genome> generator_first_organism();
    void calculate_all_fitness();
    
    std::vector<std::shared_ptr<Organism>> crossover_pool;
    std::vector<std::shared_ptr<Organism>> organisms;
  protected:
    int input_size;
    int output_size;
    int generate_size;
    int population_size;  

    void spawn(std::shared_ptr<Genome> g, int size);
    void separate_species();

    void sort_all_organism();
    void reproduce_agitation(); //負責進行基因附近的小範圍探索
    void reproduce();
    void mutation();
    void crossover();
    void natural_seletion();
    int applyGemoneId();
    int applySpeciesId();
    void initializeSpeciesId();
    void putOrganism(std::shared_ptr<Organism> org);
    void organism_growth_up();
  public:
    Population();
    Population(int inputSize, int outputSize,int population_size);
    ~Population();
    void evolution();

    void showInfo();
    void save_best_organism(char *filename);
};

bool organisms_order_by_fitness_and_race(std::shared_ptr<Organism> i, std::shared_ptr<Organism> j);

#endif