#ifndef TYGR_POPULATION_HPP
#define TYGR_POPULATION_HPP

#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <omp.h>
#include <fstream>

#include "Innovation.hpp"
#include "GeneLink.hpp"
#include "GeneNode.hpp"
#include "Organism.hpp"
#include "GeneInfoController.hpp"
#include "GeneModelSaver.hpp"

class GeneLink;
class GeneNode;
class Innovation;
class GeneInfoController;

class Population
{
 private:

    int genome_id;
    int species_idx;
    bool enable_growth;

    std::vector<std::shared_ptr<Organism>> reproduce_pool;
    std::vector<std::shared_ptr<Organism>> organisms;
    std::vector<std::shared_ptr<Organism>> new_organisms_pool;

    std::shared_ptr<Genome> generator_fully_connection_genome();

    void calculate_all_fitness();
    void calculate_new_organisms_fitness();
    void reproduce_to_pool();
    int repeat_count(int gid);
    
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
    void mutation_node(std::shared_ptr<Organism> org);
    void mutation_link(std::shared_ptr<Organism> org);
    void disturb_at_pool();
    bool IsEnableGroth();
  public:
    Population();
    Population(int inputSize, int outputSize,int population_size);
    ~Population();

    std::shared_ptr<Genome> generator_first_organism();
    void evolution();
    void showInfo();
    void report_out();
    void enableGrowthState();
    void disableGrothState();
    void save_best_organism(char *filename);
};

bool organisms_order_by_fitness_and_race(std::shared_ptr<Organism> i, std::shared_ptr<Organism> j);

#endif