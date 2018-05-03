#include "Population.hpp"

#include <memory>
#include <iostream>
#include "Innovation.hpp"
#include "GeneInfoController.hpp"
#include "GeneModelSaver.hpp"

using namespace std;

void test_innovation();
void save_model_sample();

int main()
{
    int inputSize = 784;
    int outputSize = 10;
    int population_size = 100;
    const int evolution_time = 100000;
    auto pop = make_unique<Population>(inputSize, outputSize, population_size);
    pop->enableGrowthState();

    pop->showInfo();
    cout << "***************************start***************************" << endl;
    for (int i = 0; i < evolution_time; i++)
    {
        cout << "evolution [" << i << "/" << evolution_time << "]";
        pop->evolution();
        pop->showInfo();
    }

    auto info_control = GeneInfoController::getInstance();
    info_control->showInfo();
    auto innovation = Innovation::getInstance();
    innovation->showInfo();

    return 0;
}

void test_innovation()
{
    cout << Innovation::getInstance()->applyLinkInnovation() << endl;
    cout << Innovation::getInstance() << endl;
    cout << Innovation::getInstance()->applyNodeInnovation() << endl;
    cout << Innovation::getInstance()->applyNodeInnovation() << endl;
    Innovation::getInstance()->showInfo();
}
