#include "NerveNetwork.hpp"

using namespace std;

NerveNetwork::NerveNetwork(shared_ptr<Genome> g)
{
    this->fitness = 0;
    this->genome = g->clone();
    // cout << "constructor" << endl;
    this->establishedComponent();
}

NerveNetwork::~NerveNetwork()
{
}

void NerveNetwork::establishedComponent()
{
    // cout << "established component." << endl;
    this->establishedNodes();
    // cout << "established nodes." << this->in_nodes.size() << "-" << this->hidden_nodes.size() << "-" << this->out_nodes.size() << endl;
    this->establishedLinks();
    // cout << "establish done." << endl;
}

std::shared_ptr<Genome> NerveNetwork::toGenome()
{
    for (auto const &link : this->links)
    {
        int innov = link->getInnovationId();

        for (auto const &g_link : this->genome->links)
        {
            //如果找到對應的基因連結則將演化過後的權重紀錄回到基因之中
            if (innov == g_link->InnovationId())
            {
                g_link->weight = link->getWeight(); // 將以修正完的權重修正至基因之中
                break;
            }
        }
    }

    return this->genome;
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
    // cout << "established links [" << this->genome->genomme_id << "]" << endl;
    // cout << "this->genome->links " << this->genome->links.size() << endl;
    for (auto const &link_info : this->genome->links)
    {
        if (!link_info->isEnable)
            continue;

        auto in_node = this->find_neuron(link_info->inNodeId);
        auto out_node = this->find_neuron(link_info->outNodeId);
        auto w = link_info->weight;
        // cout << link_info->innovationId << " inNodeId  = " << link_info->inNodeId << "\t"
        //      << "outNodeId = " << link_info->outNodeId << endl;

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
void NerveNetwork::train_SGD(int times)
{
    for (int i = 0; i < times; i++)
    {
        auto data_helper = make_shared<DataHelper>();
        data_helper->TrainingMode();

        for (int j = 0; j < data_helper->batch_size(); j++)
        {
            this->inference(data_helper);
            int idx = 0;
            double error = 0;
            for (auto const &out : this->out_nodes)
            {
                error = this->accumulate_error[idx++];
                //cout << "train[ "<< i <<" ] ... <error>=" << error << endl;
                out->notifyError(error);
                out->adjust_all();
            }
            // cout << this->genome->genomme_id << " train time = [" << i << "/" << times << "]"
            //      << "\tloss " << loss << endl;
        }
    }
}
void NerveNetwork::train(int times)
{
    // double total_loss = 0;
    for (int i = 0; i < times; i++)
    {
        this->inference(true);
        // this->inference(true);
        // cout << this->genome->genomme_id << " train time = [" << i << "/" << times << "]"
        //      << "\tloss " << loss << endl;

        int idx = 0;
        double error = 0;
        for (auto const &out : this->out_nodes)
        {
            error = this->accumulate_error[idx++];
            //cout << "train[ "<< i <<" ] ... <error>=" << error << endl;
            out->notifyError(error);
            out->adjust_all();
        }
    }
}

double NerveNetwork::get_train_accuracy()
{
    return get_accuracy(true);
}

double NerveNetwork::get_inference_accuracy()
{
    return get_accuracy(false);
}

double NerveNetwork::get_accuracy(bool train_mod)
{
    // cout << "get_accuracy mod = " << train_mod << endl;
    auto data_helper = make_shared<DataHelper>();
    if (train_mod)
        data_helper->TrainingMode();
    else
        data_helper->InferenceMode();

    int batch_size = data_helper->batch_size();
    double accuracy_total = 0;
    // cout << "batch size = " << batch_size << endl;
    for (int i = 0; i < batch_size; i++)
    {
        // cout << "start batch" << endl;
        auto inputs = data_helper->getInputs();
        auto desires = data_helper->getOutputs();

        int idx = 0;
        for (auto const &node : this->in_nodes)
            node->feed(inputs[idx++]);

        // cout << "feed data done." << endl;
        idx = 0;
        vector<double> outputs;
        for (auto const &node : this->out_nodes)
        {
            double out = node->getAxon();
            outputs.push_back(out);
        }
        // cout << "get output data done." << endl;
        
        int desire_size = desires.size();
        int predit_size = outputs.size();

        if (desire_size != predit_size)
        {
            cout << "desire size = " << desire_size << "\tpredit size = " << predit_size << endl;
            continue;
        }

        int max_predit_idx = 0, max_label_idx = 0;
        double temp_predit_val = outputs[0];
        double temp_label_val = desires[0];
        for (int j = 1; j < desire_size; j++)
        {
            double out_val = outputs[j];
            double label_val = desires[j];
            // cout << "output " << out_val << " temp = " << temp_predit_val << endl;
            if (out_val > temp_predit_val)
            {
                temp_predit_val = out_val;
                max_predit_idx = j;
            }

            if (label_val > temp_label_val)
            {
                temp_label_val = label_val;
                max_label_idx = j;
            }
            // cout << "[" << j << "] output " << out_val << " temp = " << temp_predit_val << endl;
            // cout << "[" << j << "] label " << label_val << " temp = " << temp_label_val << endl;
        }

        if ((max_label_idx < 0) || (max_predit_idx < 0))
            cout << "!!!!" << endl;

        //預測成功
        if (max_predit_idx == max_label_idx)
            accuracy_total += 1.0;
        else
        {
        } //預測失敗

        // cout << "loop => [" << accuracy_total << "/" << i << "/" << batch_size << "]\tpredit = "
        //      << max_predit_idx << " label = " << max_label_idx << endl
        //      << endl;

        //狀態清理,以便處理下一筆資料
        this->nodes_recover();
        data_helper->move_next();
    }

    double avg_accuracy = accuracy_total / batch_size;
    // cout << "avg acc = [" << accuracy_total << "/" << batch_size << "]=" << avg_accuracy << endl
    //      << endl;

    return avg_accuracy;
}

double NerveNetwork::inference(std::shared_ptr<DataHelper> data_helper)
{
    this->accumulate_error.clear();
    //執行一個batch size 的檢測
    double loss = 0;
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
    loss += abs(mseHelper->getloss());

    //儲存error 以利後續進行 bp 修正
    for (auto const &error : mseHelper->getErrors())
    {
        this->accumulate_error.push_back(error);
    }

    //狀態清理,以便處理下一筆資料
    this->nodes_recover();
    data_helper->move_next();

    return loss;
}

double NerveNetwork::inference(bool isTrain)
{
    this->accumulate_error.clear();
    //執行一個batch size 的檢測

    double loss = 0;
    //建立資料處理器,協助取得資料
    auto data_helper = make_shared<DataHelper>();

    if (isTrain)
        data_helper->TrainingMode();
    else
        data_helper->InferenceMode();

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
        loss += abs(mseHelper->getloss());

        //儲存error 以利後續進行 bp 修正
        for (auto const &error : mseHelper->getErrors())
        {
            this->accumulate_error.push_back(error);
        }

        //狀態清理,以便處理下一筆資料
        this->nodes_recover();
        data_helper->move_next();
    }

    double avg_loss = (loss / batch_size);
    // double avg_loss = loss;
    // cout << "[" << this->genome->genomme_id << "]\tloss at inference function " << loss << "\tbatch size = " << batch_size << "\t  avg_loss = " << avg_loss << endl;
    return avg_loss;
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

void NerveNetwork::show_info()
{
}