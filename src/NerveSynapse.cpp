#include "NerveSynapse.hpp"

using namespace std;

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

void NerveSynapse::calculate_delta(double loss)
{
    cout << "synapse calculate_delta ..." << endl;
    if (this->outNode->IsOutputNode())
    {
        this->delta_weight = loss * this->weight;
        cout << "ounode calculate_delta" << endl;
    }
    else
    {
    }
}

void NerveSynapse::update_weight()
{
    cout << "update weight --> detla weight = " << this->delta_weight << endl;

    this->weight += this->delta_weight; // update weights
    this->delta_weight = 0;             // clear delta weight
}

double NerveSynapse::getWeight()
{
    return this->weight;
}