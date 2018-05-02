#include "Population.hpp"

#include <memory>
#include <iostream>
#include "Innovation.hpp"
#include "GeneInfoController.hpp"
#include "GeneModelSaver.hpp"

using namespace std;

void test_innovation();

int main()
{
    int inputSize = 784;
    int outputSize = 10;
    int population_size = 100;
    const int evolution_time = 10000;
    auto pop = make_unique<Population>(inputSize, outputSize, population_size);

    auto basic_g = pop->generator_first_organism();
    
    cout << "save start." << endl;
    auto model_saver = make_unique<GeneModelSaver>(basic_g);
    std::string file_name("test_model_save");
    model_saver->Save(file_name);

    cout << "save done." << endl;
    // for (int i = 0; i < evolution_time; i++)
    // {
    //     cout << "evolution [" << i << "/" << evolution_time << "]";
    //     pop->evolution();
    //     pop->showInfo();
    // }

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
