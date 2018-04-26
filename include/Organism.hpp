#ifndef TYGR_ORGANISM_HPP
#define TYGR_ORGANISM_HPP

#include <memory>
#include <iostream>
#include <vector>

#include "Genome.hpp"
#include "NerveNetwork.hpp"
#include "GeneLink.hpp"
#include "GeneNode.hpp"

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
};

#endif
