#ifndef TYGR_ORGANISM_HPP
#define TYGR_ORGANISM_HPP

#include <memory>
#include <iostream>
#include <vector>

#include "Genome.hpp"
#include "NerveNetwork.hpp"

class Genome;

class Organism{
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
        double compatibility(std::shared_ptr<Organism> org);
};

#endif