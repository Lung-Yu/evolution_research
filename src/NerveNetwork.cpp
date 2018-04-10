#include "NerveNetwork.hpp"

using namespace std;

NerveNetwork::NerveNetwork(shared_ptr<Genome> g)
{
    this->fitness = 0;
    this->genome = g->clone();
    cout << "constructor" << endl;
    this->establishedComponent();
}

NerveNetwork::~NerveNetwork()
{
}

void NerveNetwork::establishedComponent()
{
    cout << "established component." << endl;
    this->establishedNodes();
    cout << "established nodes." << endl;
    this->establishedLinks();
    cout << "establish done." << endl;
}

void NerveNetwork::establishedNodes()
{
    for (auto const &node_info : this->genome->nodes)
    {
        auto node = make_shared<NerveNeuron>(node_info);

        switch (node_info->node_type)
        {
        case NODE_TYPE::Sensor:
            this->in_nodes.push_back(node);
            break;
        case NODE_TYPE::Output:
            this->out_nodes.push_back(node);
            break;
        default:
            this->hidden_nodes.push_back(node);
            break;
        }
    }
}

void NerveNetwork::establishedLinks()
{
    cout << "established links" << endl;
    cout << "this->genome->links " << this->genome->links.size() << endl;
    for (auto const &link_info : this->genome->links)
    {
        auto in_node = this->find_neuron(link_info->inNodeId);
        auto out_node = this->find_neuron(link_info->outNodeId);
        auto w = link_info->weight;

        auto new_link = make_shared<NerveSynapse>(in_node, out_node, w);
        out_node->attach(new_link); //將inNode 與 outNode 連接在一起

        this->links.push_back(new_link);
    }

    cout << "established links done." << endl;
}

shared_ptr<NerveNeuron> NerveNetwork::find_neuron(int id)
{
    for (auto const &node : this->hidden_nodes)
        if (node->getNodeId() == id)
            return node;

    for (auto const &node : this->in_nodes)
        if (node->getNodeId() == id)
            return node;

    for (auto const &node : this->out_nodes)
        if (node->getNodeId() == id)
            return node;

    return nullptr;
}

double NerveNetwork::inference()
{
    auto data_helper = make_unique<DataHelper>();

    vector<double> losses;

    for (int i = 0; i < data_helper->batch_size(); i++)
    {
        auto inputs = data_helper->getInputs();
        auto desires = data_helper->getOutputs();

        //放入資料
        int idx = 0;
        for (auto const &node : this->in_nodes)
            node->feed(inputs[idx++]);

        //運算並計算其loss值
        idx = 0;
        vector<double> outputs;
        for (auto const &node : this->out_nodes)
        {
            double out = node->getAxon();

            outputs.push_back(out);
        }

        //計算損失值
        auto mseHelper = make_unique<MeanSquaredError>(outputs, desires);
        auto loss = mseHelper->getloss();
        losses.push_back(loss);
        cout << "loss : " << loss << endl;


        this->nodes_recover();
        data_helper->move_next();
    }

    double avg_loss = 0;
    for (auto const &loss : losses)
        avg_loss += loss;
    avg_loss /= data_helper->batch_size();

    return avg_loss;
}

void NerveNetwork::nodes_recover(){
    for(auto const &node : this->in_nodes)
        node->recover();

    for(auto const &node : this->hidden_nodes)
        node->recover();

    for(auto const &node : this->out_nodes)
        node->recover();
}