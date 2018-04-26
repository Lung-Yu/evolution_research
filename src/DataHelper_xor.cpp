#include "DataHelper.hpp"

DataHelper::DataHelper()
{
    this->isTrain = true;
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

void DataHelper::TrainingMode()
{
    this->isTrain = true;
}
void DataHelper::InferenceMode()
{
    this->isTrain = false;
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