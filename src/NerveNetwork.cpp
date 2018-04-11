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
    // cout << "established nodes." << this->in_nodes.size() << "-" << this->hidden_nodes.size() << "-" << this->out_nodes.size() << endl;
    this->establishedLinks();
    cout << "establish done." << endl;
}

void NerveNetwork::establishedNodes()
{
    for (auto const &node_info : this->genome->nodes)
    {
        auto node = make_shared<NerveNeuron>(node_info);
        // cout << "establish node id " << node_info->node_id << " " << node_info->node_type << endl;
        switch (node_info->node_type)
        {
        case NODE_TYPE::Sensor:
            this->in_nodes.push_back(node);
            break;
        case NODE_TYPE::Output:
            this->out_nodes.push_back(node);
            break;
        case NODE_TYPE::Hidden:
        default:
            this->hidden_nodes.push_back(node);
            break;
        }
    }
}

void NerveNetwork::establishedLinks()
{
    // cout << "established links" << endl;
    // cout << "this->genome->links " << this->genome->links.size() << endl;
    for (auto const &link_info : this->genome->links)
    {
        auto in_node = this->find_neuron(link_info->inNodeId);
        auto out_node = this->find_neuron(link_info->outNodeId);
        auto w = link_info->weight;
        // cout << "inNodeId  = " << link_info->inNodeId << "\t"
        //      << "outNodeId = " << link_info->outNodeId << endl;
        // cout << "inNode  = " << in_node << "\t"
        //      << "outNode = " << out_node << endl
        //      << endl;

        auto new_link = make_shared<NerveSynapse>(link_info->innovationId, in_node, out_node, w);
        out_node->attach(new_link); //將inNode 與 outNode 連接在一起

        this->links.push_back(new_link);
    }

    // cout << "established links done." << endl;
}

shared_ptr<NerveNeuron> NerveNetwork::find_neuron(int id)
{
    // cout << "try to find hidden nodes." << endl;
    for (auto const &node : this->hidden_nodes)
        if (node->getNodeId() == id)
            return node;

    // cout << "try to find in nodes." << endl;
    for (auto const &node : this->in_nodes)
        if (node->getNodeId() == id)
            return node;

    // cout << "try to find out nodes." << endl;
    for (auto const &node : this->out_nodes)
        if (node->getNodeId() == id)
            return node;

    // cout << "not find nodes." << endl;
    return nullptr;
}
void NerveNetwork::train(int times)
{
    for (int i = 0; i < times; i++)
    {
        double loss = this->inference();

        int idx = 0;
        for (auto const &out : this->out_nodes)
        {
            double error = this->accumulate_error[idx++];
            out->notifyError(error);
            out->adjust_all();
        }
        cout << endl
             << "train [" << i << "/" << times << "]loss =>" << loss << endl;
    }
}
double NerveNetwork::inference()
{
    this->accumulate_error.clear();
    //執行一個batch size 的檢測

    double loss = 0;
    //建立資料處理器,協助取得資料
    auto data_helper = make_shared<DataHelper>();
    int batch_size = data_helper->batch_size();

    for (int i = 0; i < batch_size; i++)
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
        loss += mseHelper->getloss();

        //儲存error 以利後續進行 bp 修正
        for (auto const &error : mseHelper->getErrors())
            this->accumulate_error.push_back(error);

        //狀態清理,以便處理下一筆資料
        this->nodes_recover();
        data_helper->move_next();
    }

    return (loss / batch_size);
}

void NerveNetwork::nodes_recover()
{
    for (auto const &node : this->in_nodes)
        node->recover();

    for (auto const &node : this->hidden_nodes)
        node->recover();

    for (auto const &node : this->out_nodes)
        node->recover();
}