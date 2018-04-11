#include "NerveSynapse.hpp"

using namespace std;

NerveSynapse::NerveSynapse(int innov,std::shared_ptr<NerveNeuron> in, std::shared_ptr<NerveNeuron> out, double w)
{
    this->innovationId = innov;
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
    double error = loss * this->weight;
    // cout << this->innovationId << " synapse calculate_delta ..." << endl;

    this->delta_weight += loss * this->weight;

    // if (this->outNode->IsOutputNode())
    // {
    //     this->delta_weight = loss * this->weight;
    //     // cout << "ounode calculate_delta" << endl;
    // }
    // else
    // {
    //     this->delta_weight = loss * this->weight;
    //     // cout << "hiddent calculate_delta" << endl;
    // }

    this->inNode->notifyError(error);
}

void NerveSynapse::update_weight()
{
    // cout << this->innovationId << " call update weight --> detla weight = " << this->delta_weight << endl;
    this->weight += this->delta_weight; // update weights
    this->delta_weight = 0;             // clear delta weight

    this->inNode->adjust_all();
}

double NerveSynapse::getWeight()
{
    return this->weight;
}