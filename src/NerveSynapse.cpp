#include "NerveSynapse.hpp"

NerveSynapse::NerveSynapse(std::shared_ptr<NerveNeuron> in, std::shared_ptr<NerveNeuron> out, double w)
{
    this->inNode = in;
    this->outNode = out;
    this->weight = w;
}

NerveSynapse::~NerveSynapse()
{
}

double NerveSynapse::feed_forward()
{
    double result = this->inNode->getAxon() * this->weight;
    return result;
}

double NerveSynapse::getWeight()
{
    return this->weight;
}