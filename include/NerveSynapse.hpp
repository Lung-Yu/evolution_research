#ifndef TYGR_NERVE_SYNAPSE_HPP
#define TYGR_NERVE_SYNAPSE_HPP

#include <memory>
#include "NerveNeuron.hpp"

class NerveNeuron;

class NerveSynapse
{
  protected:
    int innovationId;
    int inNodeId;
    int outNodeId;
    double weight;

    std::shared_ptr<NerveNeuron> inNode;
    std::shared_ptr<NerveNeuron> outNode;

  public:
    NerveSynapse(std::shared_ptr<NerveNeuron> in,std::shared_ptr<NerveNeuron> out,double weight);
    ~NerveSynapse();
    double feed_forward();
    double getWeight();
};

#endif