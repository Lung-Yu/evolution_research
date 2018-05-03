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

    double basic_loss = 1 * NEAT::learning_rate * this->outNode->get_differential(); //學習速率 * 激勵函數的導函數
    //輸出層的節點權重
    if (this->outNode->nodeInfo->getNodeType() == NODE_TYPE::Output)
    {
        this->delta_weight = basic_loss * loss; //如果是輸出層的權重,loss 剛好就是 loss function的導函數
        this->outNode->node_delta += this->delta_weight;
    }
    else //隱藏層權重
    {
        double hidden_delta = this->outNode->calc_node_error(this->outNode->node_delta);
        this->delta_weight = basic_loss * hidden_delta;
        this->inNode->node_delta += hidden_delta;
    }

    // cout << this->inNode->node_id << "-" << this->outNode->node_id << "\t delta=" << this->delta_weight << endl;

    this->inNode->notifyError(loss);
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