#include "Population.hpp"

#include <memory>

int main(){
    
    int inputSize = 2;
    int outputSize = 1;
    int generate_size = 100;
    int population_size = 10000;
    auto pop = std::make_unique<Population>(inputSize,outputSize,generate_size,population_size);
    pop->evolution();
    pop->showInfo();

    return 0;
}