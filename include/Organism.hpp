#ifndef TYGR_ORGANISM_HPP
#define TYGR_ORGANISM_HPP

#include <memory>
#include <iostream>
#include <vector>

#include "Genome.hpp"
#include "NerveNetwork.hpp"
#include "GeneLink.hpp"
#include "GeneNode.hpp"
#include "NEAT.hpp"

class Genome;
class GeneLink;
class GeneNode;

class Organism
{
  private:
    int evolution_time;
    void evolution_fitness();

  protected:
    std::shared_ptr<Genome> gemone;
    double fitness;
    double accuracy;
    double accuracy_train;
    double loss;

  public:
    Organism(std::shared_ptr<Genome> g);
    ~Organism();

    int species_id; //種族代號
    double getFitness();
    void evolution();
    int getOrganismId();
    std::shared_ptr<Organism> clone();
    double compatibility(std::shared_ptr<Organism> org);
    std::shared_ptr<Organism> crossover(int new_org_id, std::shared_ptr<Organism> org);
    void mutationNode();
    void mutationLink();
    void growthUp();

    double getAccuracy();
    double getTrainAccuracy();
    double getLoss();
    double calculate_accuracy();
    double calculate_accuracy_train();
    double calculate_loss();
    void harass();

    std::shared_ptr<Genome> getGenome();
    void show();
};

#endif
