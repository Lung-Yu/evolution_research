#ifndef TYGR_POPULATION_HPP
#define TYGR_POPULATION_HPP

#include <vector>
#include <iostream>

#include "Innovation.hpp"
#include "GeneLink.hpp"
#include "GeneNode.hpp"
#include "Organism.hpp"
class GeneLink;
class GeneNode;

class Population
{
 private:
    int genome_id;
    int species_idx;
    std::unique_ptr<Innovation> innovation;
    std::shared_ptr<Genome> generator_fully_connection_genome();
    std::shared_ptr<Genome> generator_first_organism();

    std::vector<std::shared_ptr<Organism>> organisms;
    std::vector<std::shared_ptr<GeneLink>> links;
    std::vector<std::shared_ptr<GeneNode>> nodes;
  protected:
    int input_size;
    int output_size;
    int generate_size;
    int population_size;  

    void spawn(std::shared_ptr<Genome> g, int size);
    void separate_species();

    void reproduce();
    void mutation();
    void crossover();

    int applyGemoneId();
    int applySpeciesId();
    void initializeSpeciesId();
    void putNode(std::shared_ptr<GeneNode> node);
    void putLink(std::shared_ptr<GeneLink> node);
    int getNodeId(int node_index);
    void putOrganism(std::shared_ptr<Organism> org);

  public:
    Population();
    Population(int inputSize, int outputSize, int generate_size, int population_size);
    ~Population();
    void evolution();

    void showInfo();
};

#endif