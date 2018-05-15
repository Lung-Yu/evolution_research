#include "Population.hpp"

#include <memory>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include "Innovation.hpp"
#include "GeneInfoController.hpp"
#include "GeneModelSaver.hpp"

using namespace std;

void test_innovation();
void save_model_sample();

int main()
{
    //unsigned seed = (unsigned)time(NULL); // 取得時間序列
    //srand(97); // 以時間序列當亂數種子


    int inputSize = 4;
    int outputSize = 3;
    int population_size = 15;
    const int evolution_time = 10000;
    auto pop = make_unique<Population>(inputSize, outputSize, population_size);
    pop->enableGrowthState();
    // pop->disableGrothState();
    pop->showInfo();
    cout << "***************************start***************************" << endl;
    for (int i = 0; i < evolution_time; i++)
    {
        cout << "evolution [" << i << "/" << evolution_time << "]";
        pop->evolution();
        pop->report_out();
        // pop->showInfo();
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
