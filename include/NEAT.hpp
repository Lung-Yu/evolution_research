#ifndef TYGR_NEAT_HPP
#define TYGR_NEAT_HPP

#include <cstdlib>
namespace NEAT
{

extern double disjoint_coeff;
extern double excess_coeff;
extern double mutdiff_coeff;
extern double compat_threshold;
extern double mutation_link; //突變添加連結
extern double mutation_node; //突變添加節點
extern double survival_rate; //存活比率90% 代表最後10%的基因被淘汰

//[x,y]
int randint(int x, int y)
{
    return rand() % (y - x + 1) + x;
}

//[0,1]
double randfloat()
{
    return rand() / (double)RAND_MAX;
}

}
#endif