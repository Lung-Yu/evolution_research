#ifndef TYGR_NEAT_HPP
#define TYGR_NEAT_HPP

#include <cstdlib>

class NEAT
{
  public:
    static double disjoint_coeff;
    static double excess_coeff;
    static double mutdiff_coeff;
    static double compat_threshold;
    static double mutation_link; //突變添加連結
    static double mutation_node; //突變添加節點
    static double mutation_reduce_node ; //突變減少節點
    static double mutation_reduce_link;  //突變減少節點
    static double survival_rate; //存活比率90% 代表最後10%的基因被淘汰
    static double reproduce_rate;
    static int growth_time;
    static double learning_rate;
    static int repeat_max_count;

    static double randfloat();
    static int randint(int x, int y);

};

#endif