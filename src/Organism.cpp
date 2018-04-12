#include "Organism.hpp"

using namespace std;

Organism::Organism(std::shared_ptr<Genome> g)
{
    this->evolution_time = 100;
    this->species_id = -1;

    this->gemone = g;
}

Organism::~Organism()
{
}

void Organism::evolution()
{
    evolution_fitness();
}

void Organism::evolution_fitness()
{
    auto net = make_shared<NerveNetwork>(this->gemone);
    net->train(this->evolution_time);
    this->fitness = net->inference();
}
double Organism::compatibility(std::shared_ptr<Organism> org){
    return this->gemone->compatibility(org->gemone);
}
double Organism::getFitness()
{
    return this->fitness;
}