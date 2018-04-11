#ifndef TYGR_NERVE_SYNAPSE_HPP
#define TYGR_NERVE_SYNAPSE_HPP

#include <iostream>
#include <memory>
#include "NerveNeuron.hpp"

class NerveNeuron;

class NerveSynapse
{
  protected:
    int innovationId;
    // int inNodeId;
    // int outNodeId;
    double weight;
    double delta_weight;

    std::shared_ptr<NerveNeuron> inNode;
    std::shared_ptr<NerveNeuron> outNode;

  public:
    NerveSynapse(int innov,std::shared_ptr<NerveNeuron> in,std::shared_ptr<NerveNeuron> out,double weight);
    ~NerveSynapse();
    double feed_forward();
    double getWeight();
    void calculate_delta(double loss);
    void update_weight();
};

#endif