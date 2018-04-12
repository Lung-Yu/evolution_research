#include "NEAT.hpp"

double NEAT::disjoint_coeff = 1.0;
double NEAT::excess_coeff = 1.0;
double NEAT::mutdiff_coeff = 0.3;
double NEAT::compat_threshold = 0.4;
double NEAT::mutation_link = 0.1; //突變添加連結
double NEAT::mutation_node = 0.3; //突變添加節點
double NEAT::survival_rate = 0.4; //存活比率90% 代表最後10%的基因被淘汰

int NEAT::randint(int x, int y)
{
    return rand() % (y - x + 1) + x;
};

//[0,1]
double NEAT::randfloat()
{
    return rand() / (double)RAND_MAX;
};