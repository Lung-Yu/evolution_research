#include <iostream>
#include <vector>
#include "NerveNetwork.hpp"
#include "DataHelper.hpp"
#include "Innovation.hpp"

using namespace std;
void generate_basic_fully_connect();

DataHelper::DataHelper()
{
    this->current_idx = 0;

    std::vector<double> input_1;
    input_1.push_back(0);
    input_1.push_back(0);
    std::vector<double> output_1;
    output_1.push_back(0);
    this->input_datas.push_back(input_1);
    this->desire_datas.push_back(output_1);

    std::vector<double> input_2;
    input_2.push_back(0);
    input_2.push_back(1);
    std::vector<double> output_2;
    output_2.push_back(1);
    this->input_datas.push_back(input_2);
    this->desire_datas.push_back(output_2);

    std::vector<double> input_3;
    input_3.push_back(1);
    input_3.push_back(0);
    std::vector<double> output_3;
    output_3.push_back(1);
    this->input_datas.push_back(input_3);
    this->desire_datas.push_back(output_3);

    std::vector<double> input_4;
    input_4.push_back(1);
    input_4.push_back(1);
    std::vector<double> output_4;
    output_4.push_back(0);
    this->input_datas.push_back(input_4);
    this->desire_datas.push_back(output_4);
}

DataHelper::~DataHelper()
{
}

int DataHelper::batch_size()
{
    return 4;
}

void DataHelper::move_next()
{
    this->current_idx++;
    if (current_idx >= this->batch_size())
        this->current_idx = 0;
}

int DataHelper::getInputSize()
{
    return 2;
}

int DataHelper::getOutputSize()
{
    return 1;
}

std::vector<double> DataHelper::getInputs()
{
    return this->input_datas[this->current_idx];
}

std::vector<double> DataHelper::getOutputs()
{
    return this->desire_datas[this->current_idx];
}

void test_inference();
void test_training();
int main()
{
    test_training();

    return 0;
}

void test_inference()
{
    auto dataHelper = make_unique<DataHelper>();
    auto innov = make_unique<Innovation>();
    vector<shared_ptr<GeneNode>> nodes;
    vector<shared_ptr<GeneLink>> links;

    for (int i = 0; i < dataHelper->getInputSize(); i++)
    {
        // GeneNode(int id, NODE_TYPE type, NODE_FUNC_TYPE func);
        auto node = make_shared<GeneNode>(innov->applyNodeInnovation(), NODE_TYPE::Sensor, NODE_FUNC_TYPE::Identity);
        nodes.push_back(node);
    }

    for (int i = 0; i < dataHelper->getOutputSize(); i++)
    {
        auto node = make_shared<GeneNode>(innov->applyNodeInnovation(), NODE_TYPE::Output, NODE_FUNC_TYPE::Sigmoid);
        nodes.push_back(move(node));
    }

    for (int i = 0; i < dataHelper->getInputSize(); i++)
        for (int j = 0; j < dataHelper->getOutputSize(); j++)
        {
            auto node_1 = nodes[i]->getNodeId();
            auto node_2 = nodes[dataHelper->getInputSize() + j]->getNodeId();
            auto link = make_shared<GeneLink>(innov->applyLinkInnovation(), node_1, node_2, 1);
            links.push_back(link);
        }

    // Genome(std::vector<std::shared_ptr<GeneNode>> nodes, std::vector<std::shared_ptr<GeneLink>> links);
    auto g = make_shared<Genome>(nodes, links);

    auto net = make_shared<NerveNetwork>(g);
    double loss = net->inference();
    cout << loss << endl;
    // auto net = new NerveNetwork(g);
    // net->inference();
    cout << "done." << endl;
    // delete net;
}
void test_training()
{
    int hidden_size = 5;

    auto dataHelper = make_unique<DataHelper>();
    auto innov = make_unique<Innovation>();
    vector<shared_ptr<GeneNode>> nodes;
    vector<shared_ptr<GeneLink>> links;


    for (int i = 0; i < dataHelper->getInputSize(); i++)
    {
        // GeneNode(int id, NODE_TYPE type, NODE_FUNC_TYPE func);
        auto node = make_shared<GeneNode>(innov->applyNodeInnovation(), NODE_TYPE::Sensor, NODE_FUNC_TYPE::Identity);
        nodes.push_back(node);
    }

    for (int i = 0; i < dataHelper->getOutputSize(); i++)
    {
        auto node = make_shared<GeneNode>(innov->applyNodeInnovation(), NODE_TYPE::Output, NODE_FUNC_TYPE::Sigmoid);
        nodes.push_back(node);
    }

    for (int i = 0; i < hidden_size; i++)
    {
        auto node = make_shared<GeneNode>(innov->applyNodeInnovation(), NODE_TYPE::Hidden, NODE_FUNC_TYPE::Sigmoid);
        nodes.push_back(node);
    }

    //linking
    for (int i = 0; i < dataHelper->getInputSize(); i++)
        for (int j = 0; j < hidden_size; j++)
        {
            auto node_1 = nodes[i]->getNodeId();
            auto node_2 = nodes[dataHelper->getInputSize() + dataHelper->getOutputSize() + j]->getNodeId();
            auto link = make_shared<GeneLink>(innov->applyLinkInnovation(), node_1, node_2, 1);
            links.push_back(link);
        }
    
    for (int i = 0; i < hidden_size; i++)
        for (int j = 0; j < dataHelper->getOutputSize(); j++)
        {
            auto node_1 = nodes[dataHelper->getInputSize() + dataHelper->getOutputSize() + i]->getNodeId();
            auto node_2 = nodes[dataHelper->getInputSize() + j]->getNodeId();
            auto link = make_shared<GeneLink>(innov->applyLinkInnovation(), node_1, node_2, 1);
            links.push_back(link);
        }
    // Genome(std::vector<std::shared_ptr<GeneNode>> nodes, std::vector<std::shared_ptr<GeneLink>> links);
    auto g = make_shared<Genome>(nodes, links);

    auto net = make_shared<NerveNetwork>(g);

    net->train(200);

    // double loss = net->inference();
    // cout << loss << endl;
    // auto net = new NerveNetwork(g);
    // net->inference();
    cout << "done." << endl;
    // delete net;
}