#include "NerveSynapse.hpp"

using namespace std;

NerveSynapse::NerveSynapse(int innov, std::shared_ptr<NerveNeuron> in, std::shared_ptr<NerveNeuron> out, double w)
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

inline bool isInf(float fN)
{
    return fN == (fN + 1.0);
}

inline bool isNan(float fN)
{
    return !(fN == fN);
}

void NerveSynapse::calculate_delta(double loss)
{
    const double learning_rate = 0.1;
    double error = loss * this->weight;
    // cout << this->innovationId << " synapse calculate_delta ..." << endl;

    // if (isInf(loss))
    // {
    //     // cout << "isInf loss out range " << loss << endl;
    //     return;
    // }

    // if (isNan(loss))
    // {
    //     // cout << "isNan loss out range " << loss << endl;
    //     return;
    // }

    // if (!isfinite(loss) && !isnan(loss))
    // {
    //     cout << "loss out range " << loss << endl;
    //     return;
    // }

    // if(this->outNode->nodeInfo->getNodeType() == NODE_TYPE::Output){
    //     //this->delta_weight -= (learning_rate * loss * this->weight);
    //     this->delta_weight -= learning_rate * loss;
    //     // cout <<this->innovationId <<"\tdelta_weight" << learning_rate * loss << "\t loss = " << loss << endl;
    // }else{
    //     //this->delta_weight -= learning_rate * loss * this->inNode->output_val;
    //     // cout <<this->innovationId <<"\tdelta_weight" << learning_rate * loss << "\t loss = " << loss << endl;
    // }

    this->delta_weight -= learning_rate * loss * this->weight;
    
    // cout << this->innovationId << "\tweight = " << this->weight << "\tloss = " << loss << "\tdelta_weight " << this->delta_weight << "\terror" << error << endl;
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

int NerveSynapse::getInnovationId()
{
    return this->innovationId;
}

void NerveSynapse::update_weight()
{
    // cout << this->innovationId << " call update weight --> detla weight = " << this->delta_weight << endl;

    // if(this->delta_weight != 0)
    //     cout << "\tupdate = " << this->delta_weight;

    this->weight += this->delta_weight; // update weights
    this->delta_weight = 0;             // clear delta weight

    this->inNode->adjust_all();
}

double NerveSynapse::getWeight()
{
    return this->weight;
}