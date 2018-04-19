#include "Population.hpp"

#include <memory>
#include <iostream>
#include "Innovation.hpp"
#include "GeneInfoController.hpp"

using namespace std;

void test_innovation();

int main()
{
    int inputSize = 2;
    int outputSize = 1;
    int population_size = 200;
    auto pop = make_unique<Population>(inputSize,outputSize,population_size);

    for(int i=0;i<10000;i++)
        pop->evolution();
    pop->showInfo();




    auto info_control = GeneInfoController::getInstance();
    info_control->showInfo();
    auto innovation = Innovation::getInstance();
    innovation->showInfo();
    
    return 0;
}

void test_innovation(){
    cout << Innovation::getInstance()->applyLinkInnovation() << endl;
    cout << Innovation::getInstance() << endl;
    cout << Innovation::getInstance()->applyNodeInnovation() << endl;
    cout << Innovation::getInstance()->applyNodeInnovation() << endl;
    Innovation::getInstance()->showInfo();
}