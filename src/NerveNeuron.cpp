#include "NerveNeuron.hpp"

using namespace std;
NerveNeuron::NerveNeuron(std::shared_ptr<GeneNode> node_info)
{
    this->node_id = node_info->node_id;
    this->nodeInfo = node_info->clone();

    this->isActivity = false;
}

NerveNeuron::~NerveNeuron()
{
}

//將神經元與突觸進行連接
void NerveNeuron::attach(std::shared_ptr<NerveSynapse> link)
{
    cout << "node id = " << this->node_id << " add link" << link << endl;
    this->synapses.push_back(link);
}

void NerveNeuron::notify()
{
    this->sum_z = 0;

    //進行資料的傳遞
    for (auto const &synapse : this->synapses)
        this->sum_z += synapse->feed_forward();

    cout << "node id " << this->node_id << "  synapses siz " << this->synapses.size() << endl;

    // 將前一個神經元的資料加總完成後,執行activity function.
    this->execute_activity();
}

void NerveNeuron::notifyError(double error)
{
}

void NerveNeuron::adjust_all()
{
}

void NerveNeuron::execute_activity()
{
    switch (this->nodeInfo->func_type)
    {
    case NODE_FUNC_TYPE::Identity:
        this->output_val = identity_func(this->sum_z);
        break;
    case NODE_FUNC_TYPE::Sigmoid:
        this->output_val = sigmoid_func(this->sum_z);
        break;
    default:
        this->output_val = identity_func(this->sum_z);
        break;
    }
}

//若狀態為sensor時使用此方法放置資料,會直接覆蓋到output_val
void NerveNeuron::feed(double val)
{
    this->isActivity = true;
    this->output_val = val;
}

//取得神經輸出之資料
double NerveNeuron::getAxon()
{
    if (!this->isActivity)
    {
        this->notify(); //通知前面連結取的資料
        this->isActivity = true;
    }

    cout << "node id = " << this->node_id << ",activity status " << this->isActivity << " ---get axon---> " << output_val << " " << endl;
    return output_val;
}

int NerveNeuron::getNodeId()
{
    return this->node_id;
}

void NerveNeuron::recover()
{
    this->isActivity = false;
}

//activity
double sigmoid_func(double x)
{
    return 1.0 / (1 + exp(-1 * x));
}

double identity_func(double x)
{
    return x;
}

//adjust activity
double differential_sigmoid_func(double input, double output, double error)
{
    double delta = (1.0 - output) * output * input;
    return delta;
}

double differential_identity_func(double output, double error)
{
    return error;
}